#pragma once

class viewer::atoms::Shaders : public Magnum::GL::AbstractShaderProgram
{
public:
    explicit Shaders();

    Shaders& setTransformationMatrix(Magnum::Matrix4 const&);
    Shaders& setProjectionMatrix(Magnum::Matrix4 const&);
    Shaders& setAmbientColor(Magnum::Color3 const&);
    Shaders& setSpecularColor(Magnum::Color3 const&);
    Shaders& setLightIntensity(Magnum::Float);
    Shaders& setShininess(Magnum::Float);
    Shaders& setScale(Magnum::Float);

    Shaders& setShowId(bool);

private:
    Magnum::Int transformationMatrix_, projectionMatrix_, ambientColor_, specularColor_,
        lightIntensity_, shininess_, scale_,

        showID_;
};
