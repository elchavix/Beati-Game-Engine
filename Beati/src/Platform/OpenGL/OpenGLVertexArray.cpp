#include "bepch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Beati {

	// Probably temporary
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}

		BE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		// Generate a new Vertex Array Object (VAO)
		glGenVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);
		// Initialize the vertex buffers and index buffer vectors
		m_VertexBuffers.reserve(8); // Reserve space for 8 vertex buffers
		m_IndexBuffer = nullptr; // Initialize index buffer to nullptr

	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		// Delete the Vertex Array Object (VAO)
		glDeleteVertexArrays(1, &m_RendererID);
		m_RendererID = 0; // Reset the renderer ID
		m_VertexBuffers.clear(); // Clear the vertex buffers vector
		m_IndexBuffer.reset(); // Reset the index buffer shared pointer
	}

	void OpenGLVertexArray::Bind() const
	{
		// Bind the Vertex Array Object (VAO)
		glBindVertexArray(m_RendererID);

	}

	void OpenGLVertexArray::Unbind() const
	{
		// Unbind the Vertex Array Object (VAO)
		glBindVertexArray(0);
		// Optionally, unbind any bound vertex buffers or index buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		// Check if the vertex buffer is valid
		BE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		// Bind the Vertex Array Object (VAO)
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		// Get the index of the next available vertex attribute
		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		BE_CORE_ASSERT(indexBuffer, "Index Buffer cannot be null!");

		// Bind the Vertex Array Object (VAO)
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer; 
		glBindVertexArray(0); // Unbind the VAO after setting the index buffer
	}
}
