
layout(location = 0) in vec3 position_;
layout(location = 1) in float radius_;
layout(location = 2) in vec4 color_;
layout(location = 3) in int index_;

uniform highp mat4 transformationMatrix_;
uniform highp mat4 projectionMatrix_;
uniform highp float scale_;
uniform bool showID_;

flat out highp vec4 viewPosition;
flat out highp vec4 diffuseColor;
flat out highp float atomicRadius;

void main()
{
    viewPosition = transformationMatrix_ * vec4(position_, 1);    
    atomicRadius = radius_;

    gl_Position = projectionMatrix_ * viewPosition;
    gl_PointSize = scale_ * radius_ / gl_Position.w;

    if (showID_)
    {
        diffuseColor = vec4(
            (((index_ + 1) & 0x00FF0000) >> 16) / 255.0,
            (((index_ + 1) & 0x0000FF00) >> 8) / 255.0,
            (((index_ + 1) & 0x000000FF) >> 0) / 255.0, 1.0);
    }
    else
    {
        diffuseColor = color_;
    }
}