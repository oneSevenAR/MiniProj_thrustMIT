#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define g 9.80665
#define Mair 0.0289644
#define R 8.31432
#define p0 101325
#define T0 288.15
#define L 0.0065

struct RocketMeter {
    double time;
    double pressure;
    double velocity;
    double altitude;
    double temperature;
} readings[100];

double tempCalc(double h) {
    return T0 - L * h;
}

double pressureCalc(double h) {
    return p0 * pow(1 - (L * h / T0), (g * Mair) / (R * L));
}

double calculateAverageAltitude(int startIdx, int endIdx) {
    double sumAltitude = 0;
    int count = endIdx - startIdx + 1;

    for (int i = startIdx; i <= endIdx; i++) {
        sumAltitude += readings[i].altitude;
    }
    return sumAltitude / count;
}

double calculateAcceleration(int index) {
    if (index == 0) return 0;

    double deltaVelocity = readings[index].velocity - readings[index - 1].velocity;
    double deltaTime = readings[index].time - readings[index - 1].time;
    return deltaVelocity / deltaTime;
}

double calculateDistanceTraveled(int index) {
    double distance = 0;

    for (int i = 1; i <= index; i++) {
        double avgVelocity = (readings[i].velocity + readings[i - 1].velocity) / 2.0;
        double dt = readings[i].time - readings[i - 1].time;
        distance += avgVelocity * dt;
    }
    return distance;
}

int main() {
    char choice;
    printf("Enter 'U' for user-input simulation or 'S' for computer-simulated simulation: ");
    scanf(" %c", &choice);

    int size;

    switch (choice) {
        case 'U':
            printf("Enter the number of time and velocity data points: ");
            scanf("%d", &size);
            for (int i = 0; i < size; i++) {
                printf("Enter time and velocity for point %d: ", i + 1);
                scanf("%lf %lf", &readings[i].time, &readings[i].velocity);
            }
            break;

        case 'S':
            size = 82;
            double predefinedVelocity[] = {0.0, 98.46, 204.95, 317.62, 430.09, 532.77, 608.76, 625.13, 597.49, 570.19, 544.75, 520.96, 498.61, 477.54, 457.58, 439.15, 422.83, 408.13, 394.66, 382.17, 370.48, 359.46, 348.99, 338.99, 329.57, 320.65, 312.19, 304.11, 296.39, 288.95, 281.72, 274.68, 267.80, 261.08, 254.49, 248.02, 241.65, 235.37, 229.18, 223.06, 216.99, 211.02, 205.12, 199.28, 193.51, 187.81, 182.16, 176.56, 171.01, 165.51, 160.06, 154.64, 149.27, 143.93, 138.62, 133.35, 128.11, 122.89, 117.71, 112.55, 107.41, 102.29, 97.20, 92.12, 87.07, 82.03, 77.01, 71.99, 67.00, 62.02, 57.05, 52.10, 47.15, 42.22, 37.29, 32.38, 27.48, 22.58, 17.70, 12.82, 7.96, 3.11};
            double predefinedTime[] = {0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 10.5, 11.0, 11.5, 12.0, 12.5, 13.0, 13.5, 14.0, 14.5, 15.0, 15.5, 16.0, 16.5, 17.0, 17.5, 18.0, 18.5, 19.0, 19.5, 20.0, 20.5, 21.0, 21.5, 22.0, 22.5, 23.0, 23.5, 24.0, 24.5, 25.0, 25.5, 26.0, 26.5, 27.0, 27.5, 28.0, 28.5, 29.0, 29.5, 30.0, 30.5, 31.0, 31.5, 32.0, 32.5, 33.0, 33.5, 34.0, 34.5, 35.0, 35.5, 36.0, 36.5, 37.0, 37.5, 38.0, 38.5, 39.0, 39.5, 40.0, 40.5};
            for (int i = 0; i < size; i++) {
                readings[i].time = predefinedTime[i];
                readings[i].velocity = predefinedVelocity[i];
            }
            break;

        default:
            printf("Invalid choice entered.\n");
            return 1;
    }

    int interval;
    printf("Enter the desired time interval in seconds: ");
    scanf("%d", &interval);

    for (int i = 0; i < size; i++) {
        if (i > 0) { 
            double dt = readings[i].time - readings[i - 1].time;
            double avgVelocity = (readings[i].velocity + readings[i - 1].velocity) / 2.0;
            readings[i].altitude = readings[i - 1].altitude + avgVelocity * dt;
        }
        readings[i].temperature = tempCalc(readings[i].altitude);
        readings[i].pressure = pressureCalc(readings[i].altitude);

        if (fabs(fmod(readings[i].time, interval)) < 1e-9) {
            printf("Time: %.2f s, Altitude: %.2f m, Velocity: %.2f m/s, Temperature: %.2f K, Pressure: %.2f Pa\n",
                   readings[i].time, readings[i].altitude, readings[i].velocity, readings[i].temperature, readings[i].pressure);
            sleep(interval);
        }
    }

    // Remaining code for average altitude, acceleration, and distance unchanged.
    return 0;
}
