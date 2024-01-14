Smart Home & Fire System (IoT)

This project implements a Smart Home & Fire System using the ESP32 microcontroller programmed in C++ for IoT functionalities. The system is equipped with various sensors and actuators to monitor and control different aspects of a smart home.
Key Features

    Firebase Integration: Utilizes Firebase Realtime Database for storing and retrieving data, providing a seamless connection between the IoT devices and a mobile app.
    Sensors & Actuators: Includes sensors like Flame Sensor and LDR (Light Dependent Resistor), as well as actuators like Servo Motors to control door and window status.
    User Authentication: Implements user authentication using Firebase Authentication for securing access to the smart home system.
    Flutter Mobile App: The Flutter-based mobile app connects to the Firebase database, allowing users to monitor and control the smart home remotely.

Code Overview

The project consists of three main parts:

    ESP32 Code (C++): The code uploaded to the ESP32 microcontroller, enabling it to interact with sensors, actuators, and Firebase.

    Flutter Mobile App (Dart): Implements the user interface for remote monitoring and control, connecting to Firebase for real-time updates.

    Firebase Realtime Database: Stores and synchronizes data between the ESP32 devices and the Flutter mobile app.

Setup

    ESP32 Setup:
        Configure WiFi and Firebase credentials in the ESP32 code.
        Upload the code to the ESP32 microcontroller.

    Firebase Setup:
        Create a Firebase project and set up the Realtime Database.
        Configure API keys, user credentials, and database URL in the code.

    Flutter Setup:
        Update Flutter dependencies in the mobile app.
        Connect the Flutter app to the Firebase project using the provided API keys.

Usage

    ESP32 Operation:
        The ESP32 monitors sensors, detects events (e.g., flame detection), and updates the Firebase database.
        Actuators (e.g., servos controlling doors and windows) respond to commands from the mobile app.

    Mobile App:
        Users can log in, view real-time data from sensors, and control actuators remotely.
        Receive alerts on the mobile app for critical events, such as fire detection.
