#version 420
uniform double dTime;
uniform uvec2 iResolution;

out vec4 FragColor;

// raymarching sdf's
// 0.  define camera position and target in world space
//     define signed distance function for the scene
//       sdf(p) = for given point p, return distance to scene surface
// 1.  map current pixel from screen space to world space
// 2.  calculate camera ray direction for current pixel
// 3.  raymarch - get distance to scene surface along ray
// 3.1  from cam position, evaluate the sdf for the distance to nearest scene surface
// 3.2  if distance is within some threshold, surface is hit, return distance
// 3.3  else "march" that distance along the ray and evaluate sdf again
// 3.4  rinse and repeat until hit. if surface is not hit after some number of steps,
//        return "not hit"
// 4.  shade the pixel accordingly (e.g. lambertian model)
// 5.  ?????
// 6.  PROFIT!

#define MAX_STEPS 300
#define DIST_THRES 0.00001

float fTime = float(dTime);

float rand(float n) { return fract(sin(n) * 43758.5453123); }

float noise(float p) {
	float fl = floor(p);
    float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}

// calculate ray direction in world space
vec3 camRayDir(vec2 uv, vec3 camPos, vec3 camTarget) {
    vec3 camFwd = normalize(camTarget - camPos);
    vec3 camRight = normalize(cross(camFwd, vec3(0., 1., 0.)));
    vec3 camUp = normalize(cross(camFwd, camRight));
    
    float fPers = 2.+sin(fTime*0.5);
    return normalize(uv.x * camRight + uv.y * camUp + camFwd * fPers);
}

// map screen space to world space
// [0 .. res.x],[0 .. res.y] => [-res.x/res.y .. res.x/res.y],[-1 .. 1]
vec2 screenToWorld(vec2 scr) {
    return -(2. * scr - iResolution.xy) / iResolution.y;
}

float sdSphere(vec3 p, float r) {
    return length(p) - r;
}

float sdPlane(vec3 p, vec4 n) { // ??
    return dot(p, n.xyz) + n.w;
}

// distance field, the scene
// returns distance from p to nearest surface
float dist(vec3 p) {
    float sphX = noise(sin(fTime*4323.57482))*.3*sin(fTime*.1);
    float sphY = noise(sin(fTime*3047.20945))*.3*sin(fTime*.1);
    float sphZ = 15.+noise(sin(fTime*5381.29437))*.3*sin(fTime*.1);
    float scene = min(
                       sdSphere(p - vec3(sphX, sphY, sphZ), 3.0),
                       sdPlane(p, vec4(0., 1.,0.,5.))
                     );
    scene = min(scene, sdSphere(p - vec3(sin(fTime*1.5)*5., sin(fTime*1.66)*5., cos(fTime)*5.+15.), 0.4));
    return scene;
}

// calculate surface normal
// p = point on surface
vec3 normal(vec3 p) {
    float center = dist(p);
    vec2 eps = vec2(.0001, 0.);
    return normalize(vec3(dist(p + eps.xyy), dist(p + eps.yxy), dist(p + eps.yyx)) - center);
}

// casts and marches a ray in world space and
// returns a distance from camera to scene surface
// returns -1 if ray doesn't seem to hit anything
float raymarch(vec3 origin, vec3 dir) {
    float d = 0.;
    for (int i = 0; i < MAX_STEPS; i++) {
        float res = dist(origin + dir * d);
        if (res <= DIST_THRES) {
            return d;
        }
        d += res;
    }
    return -1.;
}

// render a pixel, return color value
vec3 render(vec3 camPos, vec3 camDir) {
    vec3 col;
    vec3 light1Pos = vec3(sin(fTime*1.5)*5., sin(fTime*1.66)*5., cos(fTime)*5.+15.);
    vec3 light2Pos = vec3(15., 20., 3.5);
    float surDist = raymarch(camPos, camDir);
    
    if (surDist == -1.) { // render background
        col = vec3(.3, .36, .60) - (camDir.y);
    } else { // render surface
        vec3 sur = camDir * surDist;
        vec3 surfaceCol = vec3(.62, .79, .49);
        vec3 l1 = normalize(light1Pos - sur);
        vec3 l2 = normalize(light2Pos - sur);
        float n1 = max(dot(normal(sur), l1), 0.);
        float n2 = max(dot(normal(sur), l2), 0.);
        vec3 lAmbient = vec3(.35, .38, .29);
        vec3 l1Direct = vec3(.6*abs(sin(fTime*3.)), .0, .0) * n1;
        vec3 l2Direct = vec3(0.38, .35, .32) * n2;
        
        col = surfaceCol * (l1Direct + l2Direct + lAmbient);
    }
    return col;
}

void main() {
    vec3 camPos = vec3(0., sin(fTime*0.7), -1.);
    vec3 camTar = vec3(0.,1.,15.);
    
    vec2 uv = screenToWorld(gl_FragCoord.xy);
    vec3 rd = camRayDir(uv, camPos, camTar);

    vec3 col = render(camPos, rd);
    col = pow(col, vec3(0.4545)); // gamma correction

    FragColor = vec4(col, 1.0);
}
