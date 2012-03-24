#version 120 
#extension GL_EXT_geometry_shader4 : enable

uniform float x_val;

void main(void)
{
	
    //increment variable
    int i;
    
    vec4 zero_color = vec4(0,0,0,0);
    
        
    //create lines
    for(i=0; i< gl_VerticesIn; i++){

        vec4 orig_point = gl_PositionIn[i];
        vec4 proj_point = orig_point;
        // FIXME: pass numCols as uniform, so x_val can be passed in normalized coords
        float alpha = pow((x_val+1.0)/2.0,0.5);

        gl_FrontColor=vec4(0.0,(1.0+orig_point.y)/2.0,0.0,alpha);
        gl_Position = gl_ModelViewProjectionMatrix*orig_point;
        EmitVertex();

        // original point projected onto the xz plane
        gl_FrontColor = zero_color;
        proj_point.y = 0;
        gl_Position = gl_ModelViewProjectionMatrix*proj_point;
        EmitVertex();
    }
    EndPrimitive();	
}