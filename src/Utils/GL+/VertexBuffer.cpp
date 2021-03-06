#include "VertexBuffer.h"

namespace GL {

    VertexBuffer::VertexBuffer() {
        create();
    }

    VertexBuffer::VertexBuffer(Target target, Usage usage) {
        setTarget(target);
        setUsage(usage);
    }

    VertexBuffer::~VertexBuffer() {
        destroy();
    }

    void VertexBuffer::bind() {
        glBindBuffer(static_cast<GLenum>(getTarget()), _vboID);
    }

    void VertexBuffer::bind(Target target) {
        setTarget(target);
        bind();
    }

    void VertexBuffer::unbind() {
        glBindBuffer(static_cast<GLenum>(getTarget()), 0);
    }
    
    void VertexBuffer::resize(GLsizeiptr size) {
        glBufferData(static_cast<GLenum>(getTarget()), size, nullptr, static_cast<GLenum>(getUsage()));
    }

    void VertexBuffer::setData(GLsizeiptr size, const GLvoid* data) {
        glBufferData(static_cast<GLenum>(getTarget()), size, data, static_cast<GLenum>(getUsage()));
    }

    void VertexBuffer::setData(GLsizeiptr size, const GLvoid* data, Usage usage) {
        setUsage(usage);
        glBufferData(static_cast<GLenum>(getTarget()), size, data, static_cast<GLenum>(getUsage()));
    }

    void VertexBuffer::setSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data) {
        glBufferSubData(static_cast<GLenum>(getTarget()), offset, size, data);
    }

    void VertexBuffer::setUsage(Usage usage) {
        _usage = usage;
    }

    void VertexBuffer::setTarget(Target target) {
        _target = target;
    }

    GLuint VertexBuffer::getID() const {
        return _vboID;
    }
    
    VertexBuffer::Usage VertexBuffer::getUsage() const {
        return _usage;
    }

    VertexBuffer::Target VertexBuffer::getTarget() const {
        return _target;
    }

    void VertexBuffer::create() {
        glGenBuffers(1, &_vboID);
    }

    void VertexBuffer::destroy() {
        glDeleteBuffers(1, &_vboID);
    }

}