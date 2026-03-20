#include "Renderer2D.h"
#include "Graphics/Buffers/VertexArray.h"
#include "Graphics/Shader.h"
#include "Core/Log.h"
#include <glad/glad.h>
#include <algorithm>

namespace Tassathras {

    struct LineVertex {
        glm::vec3 position;
        glm::vec4 color;
    };

    struct Renderer2DData {
        static const uint32_t maxQuads = 10000;
        static const uint32_t maxVertices = maxQuads * 4;
        static const uint32_t maxIndices = maxQuads * 6;
        static const uint32_t maxTextureSlots = 16; // Зависит от GPU

        std::shared_ptr<VertexArray> quadVAO;
        std::shared_ptr<VertexBuffer> quadVBO;
        std::shared_ptr<Shader> textureShader;
        std::shared_ptr<Texture2D> whiteTexture;

        uint32_t quadIndexCount = 0;
        std::vector<Quad> quadBuffer; 

        std::array<std::shared_ptr<Texture2D>, maxTextureSlots> textureSlots;
        uint32_t textureSlotIndex = 1; 

        static const uint32_t maxLines = 5000;
        static const uint32_t maxLineVertices = maxLines * 2; 
        std::shared_ptr<VertexArray> lineVAO;
        std::shared_ptr<VertexBuffer> lineVBO;
        std::shared_ptr<Shader> lineShader;
        uint32_t lineVertexCount = 0;
        LineVertex* lineVBOBase = nullptr;
        LineVertex* lineVBOPtr = nullptr;

        glm::mat4 viewProjectionMatrix;
    };

    static Renderer2DData s_Data;
    bool Renderer2D::s_SortingEnabled = true;

    void Renderer2D::init() {
        s_Data.quadVAO = std::make_shared<VertexArray>();

        s_Data.quadVBO = std::make_shared<VertexBuffer>(Renderer2DData::maxVertices * sizeof(QuadVertex));

        s_Data.quadVBO->setFormat({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float,  "a_TexIndex" } 
            });
        s_Data.quadVAO->addVertexBuffer(s_Data.quadVBO);

        uint32_t* indices = new uint32_t[Renderer2DData::maxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < Renderer2DData::maxIndices; i += 6) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += 4;
        }
        s_Data.quadVAO->setIndexBuffer(std::make_shared<IndexBuffer>(indices, Renderer2DData::maxIndices));
        delete[] indices;

        s_Data.quadBuffer.reserve(Renderer2DData::maxQuads);


        s_Data.lineVAO = std::make_shared<VertexArray>();

        s_Data.lineVBO = std::make_shared<VertexBuffer>(Renderer2DData::maxLineVertices * sizeof(LineVertex));

        s_Data.lineVBO->setFormat({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" }
            });
        s_Data.lineVAO->addVertexBuffer(s_Data.lineVBO);

        s_Data.lineVBOBase = new LineVertex[Renderer2DData::maxLineVertices];
        s_Data.lineVBOPtr = s_Data.lineVBOBase; 


        s_Data.whiteTexture = std::make_shared<Texture2D>(1, 1);
        uint32_t whiteData = 0xffffffff;
        s_Data.whiteTexture->setData(&whiteData, sizeof(uint32_t));

        s_Data.textureSlots[0] = s_Data.whiteTexture;

        s_Data.textureShader = std::make_shared<Shader>("assets/shaders/Texture2D.glsl");
        s_Data.lineShader = std::make_shared<Shader>("assets/shaders/Line.glsl");

        int samplers[Renderer2DData::maxTextureSlots];
        for (uint32_t i = 0; i < Renderer2DData::maxTextureSlots; i++) {
            samplers[i] = i;
        }

        s_Data.textureShader->bind();
        s_Data.textureShader->setIntArray("u_Textures", samplers, Renderer2DData::maxTextureSlots);
    }

    void Renderer2D::beginScene(const glm::mat4& viewProjectionMatrix) {
        s_Data.viewProjectionMatrix = viewProjectionMatrix;

        s_Data.textureShader->bind();
        s_Data.textureShader->setMat4("u_ViewProjection", viewProjectionMatrix);

        s_Data.lineShader->bind();
        s_Data.lineShader->setMat4("u_ViewProjection", viewProjectionMatrix);

        startBatch();
    }

    void Renderer2D::startBatch() {
        s_Data.quadIndexCount = 0;
        s_Data.quadBuffer.clear();
        s_Data.textureSlotIndex = 1;

        s_Data.lineVertexCount = 0;
        s_Data.lineVBOPtr = s_Data.lineVBOBase;
    }

    void Renderer2D::drawQuadIsometric(const glm::vec2& screenPos, const glm::vec2& size,
        const std::shared_ptr<Texture2D>& texture, float zOrder) {

        if (s_Data.quadIndexCount >= s_Data.maxIndices) nextBatch();

        float textureIndex = 0.0f;
        for (uint32_t i = 0; i < s_Data.textureSlotIndex; i++) {
            if (*s_Data.textureSlots[i] == *texture) {
                textureIndex = i;
                break;
            }
        }

        if (textureIndex == 0.0f) {
            if (s_Data.textureSlotIndex >= s_Data.maxTextureSlots) nextBatch();
            textureIndex = (float)s_Data.textureSlotIndex;
            s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
            s_Data.textureSlotIndex++;
        }

        Quad quad;
        quad.vertices[0] = { { screenPos.x, screenPos.y + size.y, zOrder }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, textureIndex };
        quad.vertices[1] = { { screenPos.x + size.x, screenPos.y + size.y, zOrder }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, textureIndex };
        quad.vertices[2] = { { screenPos.x + size.x, screenPos.y, zOrder }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, textureIndex };
        quad.vertices[3] = { { screenPos.x, screenPos.y, zOrder }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, textureIndex };

        s_Data.quadBuffer.push_back(quad);
        s_Data.quadIndexCount += 6;
    }

    void Renderer2D::flush() {
        if (!s_Data.quadBuffer.empty()) {
            if (s_SortingEnabled) {
                // Сортировка по Z (от дальних к ближним)
                std::stable_sort(s_Data.quadBuffer.begin(), s_Data.quadBuffer.end(),
                    [](const Quad& a, const Quad& b) { return a.getZ() < b.getZ(); });
            }

            s_Data.quadVBO->setData(s_Data.quadBuffer.data(), s_Data.quadBuffer.size() * sizeof(Quad));

            for (uint32_t i = 0; i < s_Data.textureSlotIndex; i++)
                s_Data.textureSlots[i]->bind(i);

            s_Data.textureShader->bind();
            s_Data.quadVAO->bind();
            glDrawElements(GL_TRIANGLES, s_Data.quadIndexCount, GL_UNSIGNED_INT, nullptr);
        }

        if (s_Data.lineVertexCount > 0) {
            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.lineVBOPtr - (uint8_t*)s_Data.lineVBOBase);
            s_Data.lineVBO->setData(s_Data.lineVBOBase, dataSize);
            s_Data.lineShader->bind();
            s_Data.lineVAO->bind();
            glDrawArrays(GL_LINES, 0, s_Data.lineVertexCount);
        }
    }

    void Renderer2D::endScene() {
        flush();
    }

    void Renderer2D::nextBatch() {
        flush();
        startBatch();
    }

    void Renderer2D::shutdown() {
        delete[] s_Data.lineVBOBase;
    }

    void Renderer2D::drawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color) {
        if (s_Data.lineVertexCount + 2 > Renderer2DData::maxLineVertices) {
            nextBatch();
        }

        s_Data.lineVBOPtr->position = { start.x, start.y, 0.0f };
        s_Data.lineVBOPtr->color = color;
        s_Data.lineVBOPtr++;

        s_Data.lineVBOPtr->position = { end.x, end.y, 0.0f };
        s_Data.lineVBOPtr->color = color;
        s_Data.lineVBOPtr++;

        s_Data.lineVertexCount += 2;
    }

    void Renderer2D::drawRect(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color) {
        drawLine({ min.x, min.y }, { max.x, min.y }, color);
        drawLine({ max.x, min.y }, { max.x, max.y }, color);
        drawLine({ max.x, max.y }, { min.x, max.y }, color);
        drawLine({ min.x, max.y }, { min.x, min.y }, color);
    }


    void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        drawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor)
    {
        drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor);
    }

    void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
        if (s_Data.quadIndexCount >= Renderer2DData::maxIndices) nextBatch();

        float textureIndex = 0.0f; 

        Quad quad;
        
        quad.vertices[0] = { { position.x,          position.y,          position.z }, { 0.0f, 0.0f }, color, textureIndex };
        quad.vertices[1] = { { position.x + size.x, position.y,          position.z }, { 1.0f, 0.0f }, color, textureIndex };
        quad.vertices[2] = { { position.x + size.x, position.y + size.y, position.z }, { 1.0f, 1.0f }, color, textureIndex };
        quad.vertices[3] = { { position.x,          position.y + size.y, position.z }, { 0.0f, 1.0f }, color, textureIndex };

        s_Data.quadBuffer.push_back(quad);
        s_Data.quadIndexCount += 6;
    }

    void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor) {
        if (s_Data.quadIndexCount >= Renderer2DData::maxIndices) nextBatch();

        float textureIndex = 0.0f;
        for (uint32_t i = 0; i < s_Data.textureSlotIndex; i++) {
            if (*s_Data.textureSlots[i] == *texture) {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f) {
            if (s_Data.textureSlotIndex >= Renderer2DData::maxTextureSlots) nextBatch();
            textureIndex = (float)s_Data.textureSlotIndex;
            s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
            s_Data.textureSlotIndex++;
        }

        glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        Quad quad;
        quad.vertices[0] = { { position.x,          position.y,          position.z }, { 0.0f, 0.0f }, color, textureIndex };
        quad.vertices[1] = { { position.x + size.x, position.y,          position.z }, { 1.0f * tilingFactor, 0.0f }, color, textureIndex };
        quad.vertices[2] = { { position.x + size.x, position.y + size.y, position.z }, { 1.0f * tilingFactor, 1.0f * tilingFactor }, color, textureIndex };
        quad.vertices[3] = { { position.x,          position.y + size.y, position.z }, { 0.0f, 1.0f * tilingFactor }, color, textureIndex };

        s_Data.quadBuffer.push_back(quad);
        s_Data.quadIndexCount += 6;
    }
}