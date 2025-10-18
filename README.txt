This project will implement a liquid simulation using smoothed particle hydrodynamics methods.

This methods is particle based (not mesh) that uses kernal function to calculate properties by smoothly interpolating values in regions of the liquid.

The aim of this project is to get better with C++, parallel programming, effectivly use the standard library and create a cool visualisation.


------- ChatGPT Explaination below -------


Got it! Let’s go over the **implementation steps** for an SPH (Smoothed Particle Hydrodynamics) simulation **without code**, focusing purely on **conceptual clarity and structure**.

---

## 🧠 Overview of the Workflow

An SPH simulation progresses in discrete time steps. Each frame updates particle properties based on fluid dynamics principles using kernel-based approximations.

---

## ✅ **Step-by-Step SPH Implementation Guide**

### **Step 1: Initialize the Simulation**

* Create a list of particles with initial positions (e.g., forming a block of fluid).
* Assign each particle a velocity (usually zero at start), density, pressure, and mass.
* Define constants: smoothing length (influence radius), rest density of fluid, viscosity, pressure stiffness, gravity, and time step.

---

### **Step 2: Set Up a Spatial Acceleration Structure**

* Use a uniform grid (also called a spatial hash or cell list) to divide the simulation space into square regions.
* Each particle is inserted into the cell corresponding to its position.
* This structure allows you to quickly find neighboring particles without checking every particle against every other one.

---

### **Step 3: Find Neighbors for Each Particle**

* For each particle, search nearby grid cells (usually 3x3 surrounding cells).
* Collect all particles within a certain radius (smoothing length).
* These neighbors are used in all following calculations for that particle.

---

### **Step 4: Compute Density and Pressure**

* Using a smoothing kernel (like Poly6), compute the particle's density based on its neighbors.
* Then, compute the pressure using an equation of state, typically proportional to the difference between the current density and the rest density.

---

### **Step 5: Compute Forces**

* **Pressure Force**: Calculated from the difference in pressure between the particle and its neighbors, using the gradient of a kernel (like Spiky kernel).
* **Viscosity Force**: Depends on the velocity difference between particles, using the Laplacian of a viscosity kernel.
* **External Forces**: Include gravity, user input, etc.
* Sum these to compute the total force acting on each particle.

---

### **Step 6: Integrate Motion**

* Update each particle’s acceleration by dividing the total force by the particle’s density.
* Update velocity by integrating acceleration over time.
* Update position by integrating velocity over time.

---

### **Step 7: Enforce Boundary Conditions**

* Check if particles have moved outside the simulation bounds (e.g., walls, floor).
* Apply constraints: reflect velocity, clamp position, or apply damping to simulate energy loss on impact.

---

### **Step 8: (Optional) Surface Detection and Rendering**

* Estimate the surface (useful for visualization or surface tension effects).
* Compute surface normals if needed.
* Render particles as points, circles, or as a smoothed field (e.g., using marching squares).

---

### **Step 9: Repeat for Each Time Step**

* Repeat all steps for each simulation update (per frame or tick).
* Adjust time step size as needed for stability (often based on maximum velocity and smoothing length).

---

## 🧭 Summary of What Happens Per Timestep:

1. Rebuild spatial grid
2. For each particle:

   * Find neighbors
   * Compute density and pressure
   * Compute forces (pressure, viscosity, gravity)
   * Integrate velocity and position
   * Handle collisions
3. Render or output results

---

