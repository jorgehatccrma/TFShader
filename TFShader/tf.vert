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
    // FIXME: pass numCols as uniform, so x_val can be passed in normalized coords
    c.a = pow((x_val+1.0)/2.0,0.05);
    
    
    /*v.x = v.x * displayScale.x;*/
    v.x = d * displayScale.x;
    v.y = v.y * displayScale.y;
    v.z = v.z * displayScale.z;
    
#ifdef USE_GEOMETRY_SHADER
    gl_Position = gl_ModelViewProjectionMatrix * v;
#else
    gl_Position = v;
#endif
    gl_FrontColor = c;
}