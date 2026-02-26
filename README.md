
# 🏗 Cartesian Robot – 3-Axis Storage-Type Manipulator  
### Design and Implementation of a Pick-and-Place System Integrated with a Production Line Model

Poznan University of Technology  
Faculty of Control, Robotics and Electrical Engineering  
Institute of Robotics and Machine Intelligence  

**Supervisor:** Prof. Stefan Brock  

**Team Members:**  
- Mohammed Shehsin Thamarachalil Abdulresak 
- Azeez Oladipupo Akinlade  
- Tadiwos Andargie  

---

## 📌 Project Overview

This project presents the complete design, mechanical implementation, embedded control architecture, and PLC integration of a **3-DoF Cartesian manipulator** developed for a model production line environment.

The system functions as a compact automation training station capable of:

- Automated pick-and-place operations  
- Storage and retrieval handling  
- Real-time motion control using STM32  
- Industrial PLC synchronization  
- Safety-compliant automation execution  

The objective is to bridge theoretical robotics knowledge with practical industrial automation implementation.

---

## 🎯 System Objectives

- Design a functional 3-axis Cartesian robot  
- Achieve ±1–2 mm positioning accuracy  
- Integrate STM32 microcontroller with Siemens PLC  
- Implement safe and repeatable motion control  
- Validate performance under real operating conditions  

---

## 🏗 Mechanical Design

### Architecture
- 3-axis Cartesian configuration (X–Y–Z)
- Linear motion structure
- Modular aluminum V-slot frame

### Motion System
- **X–Y axes:** GT2 belt and pulley transmission  
- **Z-axis:** Lead screw mechanism  
- **Actuators:** NEMA 17 stepper motors  

### Technical Specifications
- Working volume: **400 mm × 400 mm × 100 mm**
- Payload capacity: **300–400 g**
- Target positioning accuracy: **±1–2 mm**
- Optimized for precision and affordability (not high-speed industrial load)

---

## ⚡ Electrical & Control System

### Control Platform
- STM32 microcontroller (CubeIDE-based firmware)
- A4988 / DRV8825 stepper drivers
- TB6612 (if DC motor used for auxiliary motion)

### Safety Features
- Limit switches (axis homing & overtravel protection)
- Emergency stop circuit
- Driver protection mechanisms

### Power System
- 12–24V main supply
- Regulated voltage rails for control logic

---

## 🔌 Industrial Communication

The manipulator integrates with a Siemens PLC system for coordinated operation.

Supported communication protocols:

- UART command interface  
- Modbus RTU  
- Modbus TCP (via LWIP where applicable)  

PLC synchronization enables:

- Coordinated pick-and-place sequences  
- Safety supervision  
- Production-line interaction  

---

## 📂 Repository Structure

```

Cartesian_Robot_3D_Model/
│
├── firmware/
│   └── stm32_f746_cartesian/
│       ├── Core/
│       ├── Drivers/
│       ├── Middlewares/
│       ├── LWIP/
│       ├── .ioc configuration
│       └── STM32CubeIDE project files
│
├── CAD/
│   ├── project_v1 v6.f3d
│   └── project_v1 v6.stl
│
├── KiCAD/
│   ├── 3dof_cartesian.kicad_sch
│   └── 3dof_cartesian.pdf
│
├── Docs/
│   ├── Cartesian_Robot_BOM.pdf
│   └── pick_and_place_project.pdf
│
└── Images/
├── CAD renders
└── Assembly and testing photos

```

---

## 💻 Firmware Overview

The firmware is structured for modular real-time control.

Implemented features:

- Axis homing routines  
- Stepper pulse generation  
- PWM-based motor control  
- Sensor feedback handling  
- PLC command parsing  
- Motion execution logic  
- Safety interlock management  

Development Environment:
- STM32CubeIDE  
- HAL drivers  
- FreeRTOS (if enabled)  
- LWIP (for TCP communication version)  

---

## 🧩 PCB & Circuit Design

Designed using KiCAD:

- Stepper driver integration
- Power distribution
- Sensor connectors
- Safety logic integration

Future improvement:
- Migration to 4-layer PCB for cleaner integration
- Improved EMI protection and power routing

---

## 📊 Bill of Materials (BOM)

Major components include:

- NEMA 17 stepper motors  
- GT2 belts & pulleys  
- Lead screw & nut  
- STM32 microcontroller board  
- A4988 motor drivers  
- Aluminum V-slot frame  
- Limit switches  
- Power supply modules  

Estimated total cost:

**~1137 PLN (~$270 USD)**

Detailed cost breakdown available in `/Docs`.

---

## 🧪 Testing & Validation

Validation procedures include:

- Static positioning accuracy measurement  
- Repeatability testing (20–50 cycles)  
- Payload stress testing up to 400 g  
- PLC-driven automation loop tests  
- Safety system validation  

Target tolerance: **±1 mm**

---

## 🔬 Educational & Research Value

This system is designed as:

- A robotics laboratory trainer  
- A PLC–microcontroller integration platform  
- A mechatronics teaching station  

Supports laboratory exercises in:

- Robotic kinematics  
- Embedded motion control  
- PLC automation programming  
- Industrial communication  
- Safety engineering  

---

## 🔮 Future Development

Planned improvements:

- Closed-loop stepper feedback system  
- Vision-based object detection  
- Advanced trajectory planning  
- ROS2 integration  
- Industrial Ethernet expansion  
- Custom 4-layer integrated PCB  

---

## 🌍 Technical Domains Covered

- Mechatronics System Design  
- Embedded Systems Engineering  
- Industrial Automation  
- PLC Integration  
- Motion Control Systems  
- CAD & Mechanical Design  
- Power Electronics  
- Real-Time Communication  

---

## 🌿 Branch Strategy

- `master` → Stable documented release  
- `main` → Development branch (experimental firmware & hardware revisions)

Recommended workflow:
- Develop in `main`
- Merge validated releases into `master`

---

## 📜 License

This project is developed for academic and research purposes at Poznan University of Technology.

For collaboration or research inquiries, please contact the project author.

---

## 👤 Author

**Mohammed Shehsin Thamarachalil Abdulresak**  
Automatic Control & Robotics  
Poznan University of Technology  

---
