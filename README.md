# 🧬 Destiny Simulator

**Destiny Simulator** is a text-based life simulation game developed in C++, inspired by popular titles like *BitLife*. The project simulates the journey of a human life from birth to death, where every decision you make shapes your character's future, stats, and ultimate destiny.

## 🌟 Overview

The game puts you in control of a character's life choices. From early childhood education to career paths and retirement, you must manage your resources and attributes to live the best life possible—or see how badly things can go!

## ✨ Key Features

* **Attribute System:** Tracks core stats: **Happiness, Health, Intelligence, and Looks**.
* **Dynamic Life Stages:** Progress through infancy, childhood, adolescence, adulthood, and old age.
* **Random Events:** Encounter unpredictable scenarios every year that require critical decision-making.
* **Career & Education:** Choose your study path to unlock specific high-paying jobs.
* **Financial Management:** Balance your income and expenses to maintain a stable lifestyle.

## 💻 OOP Concepts Applied

This project was built using **Object-Oriented Programming (OOP)** principles to ensure clean, modular, and extensible code:

1.  **Encapsulation:** Character attributes and internal logic are protected, accessed only through controlled methods (getters/setters).
2.  **Inheritance:** Used for categorizing different types of life events or NPC interactions derived from base classes.
3.  **Polymorphism:** Method overriding is used to handle how different life stages or career types process the "Advance Year" logic.
4.  **Abstraction:** Abstract classes define the blueprint for game systems like `LifeEvent` or `Activity`, hiding complex implementation details from the main loop.

## 🚀 Getting Started

### Prerequisites
* A C++ compiler (GCC/G++, Clang, or MSVC).
* C++11 standard or higher.

### Installation & Execution
1. **Clone the repository:**
   ```bash
   git clone [https://github.com/biancaa05/Destiny-Simulator.git](https://github.com/biancaa05/Destiny-Simulator.git)
