uniform int nFrame;

void main()
{   
    /*
     float val = mod(float(nFrame),1000.0)/1000.0;
     gl_FragColor = vec4(val,0.0,0.0,1.0);	
     */
    gl_FragColor = gl_Color;
}