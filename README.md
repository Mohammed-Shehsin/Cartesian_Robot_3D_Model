# Cartesian Robot 3D Model

A 3-axis Cartesian Pick-and-Place Robot designed for precision automation tasks. This open-source project includes a full mechanical CAD assembly, bill of materials, technical drawings, and image references. Built using stepper motors, timing belts, and aluminum extrusions, the robot is controlled via an STM32 microcontroller and can be extended to CNC, sorting, or pick-and-place systems.

## 📁 Project Structure
```
Cartesian_Robot_3D_Model/
├── CAD/
│   └── project_v1_v6.f3d
├── Docs/
│   ├── Cartesian_Robot_BOM.pdf
│   ├── pick_and_place_project.PDF
├── Images/
│   ├── pick_and_place_project_001.JPG to _020.JPG
│   └── Final Renders
├── README.md
```

## 🔧 Key Features
- 3 Degrees of Freedom (X, Y, Z) Cartesian design
- NEMA 17 stepper motors with GT2 belt and lead screw transmission
- STM32 microcontroller for motion control (with PLC I/O interface)
- Modular and sturdy aluminum extrusion frame
- Compact gripper for object manipulation
- Detailed mechanical drawings and part sourcing

## 🖼️ Rendered Model

### 🔄 Multi-Angle Views (Front, Back, Left, Right)
<table>
  <tr>
    <td align="center"><b>Front View</b><br>
      <img src="https://github.com/Mohammed-Shehsin/Cartesian_Robot_3D_Model/raw/master/Images/pick_and_place_project_003.JPG" width="300">
    </td>
    <td align="center"><b>Back View</b><br>
      <img src="https://github.com/Mohammed-Shehsin/Cartesian_Robot_3D_Model/raw/master/Images/pick_and_place_project_004.JPG" width="300">
    </td>
  </tr>
  <tr>
    <td align="center"><b>Left Side</b><br>
      <img src="https://github.com/Mohammed-Shehsin/Cartesian_Robot_3D_Model/raw/master/Images/pick_and_place_project_007.JPG" width="300">
    </td>
    <td align="center"><b>Right Side</b><br>
      <img src="https://github.com/Mohammed-Shehsin/Cartesian_Robot_3D_Model/raw/master/Images/pick_and_place_project_009.JPG" width="300">
    </td>
  </tr>
</table>

---

### 🔼 Top View
<p align="center">
  <img src="https://github.com/Mohammed-Shehsin/Cartesian_Robot_3D_Model/raw/master/Images/pick_and_place_project_015.JPG" width="400">
</p>

---

### 📐 Isometric View (Best Render)
<p align="center">
  <img src="https://github.com/Mohammed-Shehsin/Cartesian_Robot_3D_Model/raw/master/Images/pick_and_place_project_018.JPG" width="400">
</p>

---

### 🧩 Full Angle Composite View
<p align="center">
  <img src="https://github.com/Mohammed-Shehsin/Cartesian_Robot_3D_Model/raw/master/Images/pick_and_place_project_020.JPG" width="400">
</p>

---

More views and photos are available in the `Images/` folder.


## 🧾 Bill of Materials
Refer to `Docs/Cartesian_Robot_BOM.pdf` for full details.

### Components Overview:
- **Motors:** 3 × NEMA 17 stepper motors
- **Transmission:** GT2 20T pulleys, belts, idlers, lead screws
- **Guides:** 8mm linear rods, T8 screws, 688 bearings
- **Controller:** STM32 Nucleo-F767ZI + PLC interface

💰 **Total Estimated Cost:** ~1137 PLN

## 📐 Technical Drawing
See `Docs/pick_and_place_project.PDF` for manufacturing layout, hole placements, and mechanical assembly references.

## ⚙️ Control System
- Microcontroller: STM32 Nucleo F767ZI
- I/O Expansion: PLC Shield (X-NUCLEO-PLC01A1)
- Motor drivers and sensor interfacing supported
- Programmed via STM32CubeIDE (or compatible IDEs)

## 🚀 Getting Started

> 📌 **Note:** All project files, including CAD, images, and documentation, are located in the **`master`** branch.  
> If you do not see the files in the `main` branch, please switch to the `master` branch on GitHub.

To clone and view this project:

```bash
git clone https://github.com/Mohammed-Shehsin/Cartesian_Robot_3D_Model.git
cd Cartesian_Robot_3D_Model
```

- Open `project_v1_v6.f3d` in Autodesk Fusion 360  
- Browse `Docs/` for drawings and parts  
- Check `Images/` for physical reference photos

## 👤 Author
**Mohammed Shehsin Thamarachalil Abdulresak**  
Automation & Robotics Engineer  
Poznan University of Technology

## 📄 License
This project is licensed under the MIT License.

## 🤝 Contributions
Contributions are welcome! If you have firmware updates, STL designs, or improvements, feel free to fork the repo and submit a pull request.
