#include <stdio.h>
#include <math.h>

int main() {
    float speed = 143.0f;
    float coast_friction = 0.35f;
    float delta_time = 1.0f / 60.0f;
    float distance = 0.0f;
    int frames = 0;
    
    printf("Initial speed: %.2f u/s\n", speed);
    printf("Coast friction: %.3f\n", coast_friction);
    printf("Delta time: %.6f\n\n", delta_time);
    
    while (speed > 5.0f && frames < 1000) {
        // Apply friction (same formula as ship_physics.c)
        speed *= powf(1.0f - coast_friction, delta_time * 60.0f);
        
        // Update distance
        distance += speed * delta_time;
        
        frames++;
        
        if (frames % 30 == 0) {
            printf("Frame %d: speed = %.2f u/s, distance = %.2f units\n", frames, speed, distance);
        }
        
        if (speed < 0.5f) {
            speed = 0.0f;
            break;
        }
    }
    
    printf("\nFinal: frames=%d, time=%.2f s, distance=%.2f units, speed=%.2f u/s\n", 
           frames, frames * delta_time, distance, speed);
    
    return 0;
}
