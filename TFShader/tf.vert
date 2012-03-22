uniform float time_s;
uniform float x_val;
uniform vec3 displayScale;

const float PI = 3.14159265;

void main()
{	
    float d = x_val;
    
    vec4 c = vec4(gl_Color);
    vec4 v = vec4(gl_Vertex);
    c.g = 1.0+(v.y-0.5);
    c.r = v.y*2.0;
    c.b = -v.y*2.0;
    
    
    /*v.x = v.x * displayScale.x;*/
    v.x = d * displayScale.x;
    v.y = v.y * displayScale.y;
    v.z = v.z * displayScale.z;
    
    gl_Position = gl_ModelViewProjectionMatrix * v;
    gl_FrontColor = c;
}