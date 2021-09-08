#include <iostream>
#include <sstream>
#include <string>

enum smart_house{
    POWER = 1,
    SOCKET = 2,
    INLIGHT = 4,
    OUTLIGHT = 8,
    INHEAT = 16,
    WATERHEAT = 32,
    CONDITIONER = 64
};

int heat_water(int status, int temperature) {
    bool initStatus = status & WATERHEAT;
    if (temperature < 0) {
        status |= WATERHEAT;
    } else {
        if (temperature > 5) {
            status &= ~WATERHEAT;
        }
    }
    bool endStatus = status & WATERHEAT;
    if (initStatus != endStatus) {
        std::cout << "Water heater is " << (status & WATERHEAT ? "ON" : "OFF") << std::endl;
    }
    return status;
}

int outside_light(int status, int time, bool movement) {
    bool initStatus = status & OUTLIGHT;
    if (movement) {
        if (time > 16 || time < 5) {
            status |= OUTLIGHT;
        }
    } else {
        status &= ~OUTLIGHT;
    }
    bool endStatus = status & OUTLIGHT;
    if (initStatus != endStatus) {
        std::cout << "Outside light is " << (status & OUTLIGHT ? "ON" : "OFF") << std::endl;
    }
    return status;
}

int inside_heating(int status, int temperature) {
    bool initStatus = status & INHEAT;
    if (temperature < 22) {
        status |= INHEAT;
    }
    if (temperature >= 25) {
        status &= ~INHEAT;
    }
    bool endStatus = status & INHEAT;
    if (initStatus != endStatus) {
        std::cout << "Inside heating is " << (status & INHEAT ? "ON" : "OFF") << std::endl;
    }
    return status;
}

int conditioner(int status, int temperature) {
    bool initStatus = status & CONDITIONER;
    if (temperature > 30) {
        status |= CONDITIONER;
    }
    if (temperature <= 25) {
        status &= ~CONDITIONER;
    }
    bool endStatus = status & CONDITIONER;
    if (initStatus != endStatus) {
        std::cout << "Conditioner is " << (status & CONDITIONER ? "ON" : "OFF") << std::endl;
    }
    return status;
}

int light_inside(int status, int time, bool state) {
    bool initStatus = status & INLIGHT;
    bool changeLight = false;
    (state ? status |= INLIGHT : status &= ~INLIGHT);
    int lightTemperature = 5000;
    if (state) {
        if (time >= 16 && time <= 20) {
            lightTemperature -= 460 * (5 - (20 - time));
            changeLight = true;
        }
        if (time > 20 && time < 24) {
            lightTemperature = 2700;
        }
    }
    std::string statusON = "ON. Light temperature is ";
    statusON += std::to_string(lightTemperature);
    statusON += "K.";
    bool endStatus = status & INLIGHT;
    if ((initStatus != endStatus) || (state && (changeLight || time == 0))) {
        std::cout << "Light inside is " << (status & INLIGHT ? statusON : "OFF") << std::endl;
    }
    return status;
}

int state_main(int step, int state) {
    std::cout << "Time is " << step << ":00. Input status (temperature outside; temperature inside; outside movement yes/no; light inside on/off)";
    std::string inputLine, movement, light;
    std::getline(std::cin, inputLine);
    std::stringstream streamLine(inputLine);
    int tempInside, tempOutside;
    streamLine >> tempOutside >> tempInside >> movement >> light;
    bool moveStatus = false;
    bool lightStatus = false;
    if (movement == "yes") moveStatus = true;
    if (light == "on") lightStatus = true;

    state = heat_water(state, tempOutside);
    state = outside_light(state, step, moveStatus);
    state = inside_heating(state, tempInside);
    state = conditioner(state, tempInside);
    state = light_inside(state, step, lightStatus);

    return state;
}

int main() {
    int status = 0;
    status |= POWER;
    status |= SOCKET;
    std::cout << "POWER is ON. SOCKETS are ON." << std::endl;
    for (int j = 1; j <= 2; ++j) {
        std::cout << "Day " << j << " started.";
        for (int i = 0; i < 24; ++i) {
            status = state_main(i, status);
        }
    }

    std::cout << "The cycle is over.";
    return 0;
}
