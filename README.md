# Console-Based-Quiz-Game
A category based interactive quiz game in c++

## Project Overview
This project is a fully interactive, console-based quiz game developed in C++.  
It is designed to reinforce core Programming Fundamentals concepts including file handling, arrays, functions, conditional statements, loops, and basic game logic.

The game allows users to select categories and difficulty levels, answer timed questions, use lifelines, and track scores through a persistent leaderboard.

---

## Features

### Multiple Categories
- Science  
- Computer Science  
- History  
- Sports  
- IQ & Logic  

Each category loads questions from a separate text file.

---

### Difficulty Levels
- Easy  
- Medium  
- Hard  

Questions are filtered according to the selected difficulty level.

---

### Quiz Session
- 10 unique random questions per session  
- No repetition of questions  
- Score streak tracking  

---

### Lifelines (One-Time Use Each)
- 50/50 – removes two incorrect options  
- Skip – skip a question without penalty  
- Swap – replace the current question with another unused question  
- Extra Time – adds additional time for the current question  

---

### Timed Questions
- Base time: 10 seconds  
- Extra Time lifeline increases time  
- Exceeding the time limit counts as a wrong answer  

---

### Scoring System
- Correct answer: +10 points  
- Streak bonuses:
  - 3 correct answers → +5 bonus  
  - 5 correct answers → +15 bonus  
- Wrong answer penalties:
  - Easy → −2  
  - Medium → −3  
  - Hard → −5  
- Score never drops below zero  

---

### Review Incorrect Answers
At the end of the quiz, players can:
- Review all incorrect questions  
- Compare correct answers with selected answers  

---

### Leaderboard
- Stores player name, score, and difficulty level  
- Data saved in `high_scores.txt`  
- Displays top 5 scores automatically  

---

## Programming Concepts Used
- File handling (ifstream, ofstream)  
- Arrays and basic data structures  
- Functions and modular programming  
- Conditional statements (if, else, switch)  
- Loops (for, while)  
- Random number generation  
- Time handling  
- Input validation  

---

## Project Structure
