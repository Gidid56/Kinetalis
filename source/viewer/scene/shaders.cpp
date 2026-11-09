#include "pch.h"

#include "viewer/core.h"

#include "shaders.h"

using namespace viewer;
using namespace Magnum::GL;
using atoms::Shaders;

// TODO Compile the resources with CMake (resources.conf)

Shaders::Shaders()
{
    Shader vert{ Version::GL460, Shader::Type::Vertex };
    Shader frag{ Version::GL460, Shader::Type::Fragment };

    vert.addFile("resources/shaders/shader.vert");
    frag.addFile("resources/shaders/shader.frag");

    if (not vert.compile() or not frag.compile())
        throw std::runtime_error{ "Failed to compile atom shaders." };

    attachShaders({ vert, frag });

    if (not link())
        throw std::runtime_error{ "Failed to link atom shader program." };

    transformationMatrix_ = uniformLocation("transformationMatrix_");
    projectionMatrix_ = uniformLocation("projectionMatrix_");
    ambientColor_ = uniformLocation("ambientColor_");
    specularColor_ = uniformLocation("specularColor_");
    lightIntensity_ = uniformLocation("lightIntensity_");
    shininess_ = uniformLocation("shininess_");
    scale_ = uniformLocation("scale_");

    showID_ = uniformLocation("showID_");
}

Shaders& Shaders::setTransformationMatrix(Magnum::Matrix4 const& matrix)
{
    setUniform(transformationMatrix_, matrix);
    return *this;
}

Shaders& Shaders::setProjectionMatrix(Magnum::Matrix4 const& matrix)
{
    setUniform(projectionMatrix_, matrix);
    return *this;
}

Shaders& Shaders::setAmbientColor(Magnum::Color3 const& ambientColor)
{
    setUniform(ambientColor_, ambientColor);
    return *this;
}

Shaders& Shaders::setSpecularColor(Magnum::Color3 const& specularColor)
{
    setUniform(specularColor_, specularColor);
    return *this;
}

Shaders& Shaders::setLightIntensity(Magnum::Float lightIntensity)
{
    setUniform(lightIntensity_, lightIntensity);
    return *this;
}

Shaders& Shaders::setShininess(Magnum::Float shininess)
{
    setUniform(shininess_, shininess);
    return *this;
}

Shaders& Shaders::setScale(Magnum::Float scale)
{
    setUniform(scale_, scale);
    return *this;
}

Shaders& Shaders::setShowId(bool showID)
{
    setUniform(showID_, showID);
    return *this;
}
