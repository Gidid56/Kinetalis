
flat in highp vec4 viewPosition;
flat in highp vec4 diffuseColor;
flat in highp float atomicRadius;

uniform highp mat4 projectionMatrix_;
uniform highp vec3 ambientColor_;
uniform highp vec3 specularColor_;
uniform highp float lightIntensity_;
uniform highp float shininess_;
uniform bool showID_;

out highp vec4 fragmentColor;

vec3 phongShading(vec3 normal)
{
	const vec3 eyeDirection = normalize(-viewPosition.xyz);
	const vec3 lightDirection = eyeDirection;
	const vec3 halfDirection = normalize(lightDirection + eyeDirection);

	const float diffuse =
		max(dot(lightDirection, normal), 0.0);

	const float specular =
		pow(max(dot(halfDirection, normal), 0.0), shininess_);

	return ambientColor_
		+ lightIntensity_ * diffuse * diffuseColor.rgb
		+ specular * specularColor_;
}

void main()
{
    if (atomicRadius == 0.0) discard;

    highp vec3 sphereCoordinate;
    sphereCoordinate.xy = gl_PointCoord.xy * vec2(2, -2) + vec2(-1, 1);
    const highp float radius = dot(sphereCoordinate.xy, sphereCoordinate.xy);

    if (radius > 1.0) discard;

    sphereCoordinate.z = sqrt(1 - radius);
        
    const vec3 fragPosition = viewPosition.xyz + atomicRadius * sphereCoordinate;
    mat4 projectionMatrixTransposed = transpose(projectionMatrix_);
    float z = dot(vec4(fragPosition, 1), projectionMatrixTransposed[2]);
    float w = dot(vec4(fragPosition, 1), projectionMatrixTransposed[3]);

    gl_FragDepth = (gl_DepthRange.diff * z / w
        + gl_DepthRange.near + gl_DepthRange.far) / 2.0;
    
    if (showID_)
    {
        fragmentColor = diffuseColor;
    }
    else
    {
        fragmentColor = vec4(phongShading(sphereCoordinate), diffuseColor.w);
    }
}