# 🛰️ Drone Flight Simulation in Unreal Engine 4
This project simulates an autonomous drone performing aerial photogrammetry by flying over a 15x15 grid at a high altitude and capturing screenshots of each sector.

## 🚀 Features
- Autonomous drone movement across a defined scanning area
- Procedural grid generation (15x15)
- Aerial screenshots with sector-based filenames
- Smooth camera switching (drone ↔ player)
- Built in C++ with full UE4 integration

## 📸 Use Case
Ideal for simulating:
- Aerial mapping
- Photogrammetry workflows
- Automated surveillance or scanning drones

## 🛠 Technologies
- Unreal Engine 4 (UE4)
- C++ (Actor-based logic)
- `UCameraComponent`, `APlayerController`
- `FScreenshotRequest` API for capturing screenshots

## 🎮 How to Run
1. Add `Pepe101` actor to your UE4 level
2. Bind the input `SwitchCamera` to a key (like `C`)
3. Play the scene — the drone will fly over the map and auto-capture screenshots

## 👩‍💻 Author
**Ksenia Krutz**  
💡 Simulation Developer | 💻 C++ in UE4 | 📈 Data & AI Enthusiast

## 📫 Contact
Feel free to reach out via [GitHub](https://github.com/KseniaKrutz) or add contact info here.


## ⚡ Fun fact
This project combines computer vision, procedural environments, and drone simulation in a way that’s as geeky as it is cool 😎