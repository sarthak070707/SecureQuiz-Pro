# Qt Proctored Quiz Application

A simple desktop **multiple-choice quiz application** built with **C++ and Qt Widgets**, extended with basic **proctoring features** like fullscreen mode, focus-loss detection, and activity logging.  
Designed as a micro-project for academic use.

---

## Features

- Loads questions from an external `Quiz.txt` file  
- Multiple-choice questions with four options (A–D)  
- Automatic scoring and result display  
- Fullscreen quiz window (reduces distraction)  
- Logs user activity to `proctor_log.txt`:
  - Quiz start and end time  
  - Question navigation  
  - Selected options and correctness  
  - Focus lost events (when user switches away from quiz window)

_No timer and no webcam are used in this version, keeping it simple and easy to run on any lab PC._

---

## Technologies Used

- **Language:** C++  
- **Framework:** Qt (Qt Widgets)  
- **IDE:** Qt Creator  
- **Platform:** Desktop (Windows / Linux)

---

## Project Structure

```text
.
├── main.cpp
├── mainwindow.h
├── mainwindow.cpp
├── mainwindow.ui
├── Quiz.txt          # Question bank (required at runtime)
└── proctor_log.txt   # Generated automatically during quiz
# SecureQuiz-Pro
