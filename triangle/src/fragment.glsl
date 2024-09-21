#version 330 core
out vec4 FragColor;

void main(){
    // float v=10.0; // 10 percent  0-100  -> 0.0 and 1.0
    // vec4 c1=vec4(1.0,0.0,0.0,1.0);
    // vec4 c2=vec4(0.0,1.0,1.0,1.0);
    // vec4 m=mix(c1,c2,v/100.0);
    // FragColor = m;

//    float i=clamp(m,0.0,1.0);
//    FragColor = vec4(0.0f, 1.0f, .5+.5*i, 1.0f);
   FragColor=vec4(1.0f,0.0f,0.0f,1.0f);
} 