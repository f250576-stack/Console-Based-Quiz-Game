
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;
const int MAX_Q = 100;

int db_diff[MAX_Q];          // Stores difficulty level (1=Easy, 2=Medium, 3=Hard)
string db_question[MAX_Q];   // Stores the question text
string db_optA[MAX_Q];       // Stores Option A text
string db_optB[MAX_Q];       // Stores Option B text
string db_optC[MAX_Q];       // Stores Option C text
string db_optD[MAX_Q];       // Stores Option D text
char db_correct[MAX_Q];      // Stores correct answer letter (A, B, C, or D)
int totalLoaded = 0;
void generateFiles();
int loadData(string filename);
void displayMenu();
void playGame();
void viewLeaderboard();
void updateLeaderboard(string name, int score, string diff);
void logSession(string name, int score, int totalQ, int correctCount, int incorrectCount);
void myDelay(int seconds);
void shuffleArray(int arr[], int n);
void printQuestion(int index, bool hideWrong);
string getCurrentDateTime();

int main() {

    srand((unsigned int)time(0));
  
    generateFiles();

    int choice = 0;
 
    do {
        system("cls");
        displayMenu();
        
        if (!(cin >> choice)) {
            cin.clear();            // Clear the error flag so cin works again
            cin.ignore(1000, '\n'); // Discard the invalid input from buffer
            choice = 0;             // Set to invalid choice to trigger default case
        }
        switch (choice) {
        case 1: playGame(); break;        // Option 1: Start a new quiz game
        case 2: viewLeaderboard(); break; // Option 2: Show high scores
        case 3: cout << "Exiting..." << endl; break; // Option 3: Exit message
        default: cout << "Invalid choice!" << endl; myDelay(1); // Any other input: error
        }

    } while (choice != 3); 

    return 0; 
}
void displayMenu() {
   
    cout << "========================================" << endl;
    cout << "       QUIZ GAME (SEMESTER PROJECT)     " << endl;
    cout << "========================================" << endl;
    cout << "1. Start New Quiz" << endl;    // Option for playing
    cout << "2. View Leaderboard" << endl;  // Option to see high scores
    cout << "3. Exit" << endl;              // Option to quit
    cout << "----------------------------------------" << endl;
    cout << "Enter Choice: "; 
}
void myDelay(int seconds) {
    time_t start = time(0);
    while (time(0) - start < seconds) {
      
    }
}
string getCurrentDateTime() {
    time_t now = time(0);
    string dt = ctime(&now);
    dt = dt.substr(0, dt.length() - 1);
    
    return dt;
}
void shuffleArray(int arr[], int n) {
    // Start from the last element and work backwards
    for (int i = n - 1; i > 0; i--) {
        // Pick a random index from 0 to i (inclusive)
        // rand() % (i+1) gives a number between 0 and i
        int j = rand() % (i + 1);
        
        // SWAP arr[i] and arr[j] using a temporary variable
        int temp = arr[i];  // Save arr[i] temporarily
        arr[i] = arr[j];    // Put arr[j]'s value into arr[i]
        arr[j] = temp;      // Put the saved value into arr[j]
    }
}
void printQuestion(int index, bool hideWrong) {
   
    cout << "\n----------------------------------------" << endl;
    cout << "[Q] " << db_question[index] << endl;  
    cout << "----------------------------------------" << endl;

    if (!hideWrong) {
        cout << "A. " << db_optA[index] << endl;
        cout << "B. " << db_optB[index] << endl;
        cout << "C. " << db_optC[index] << endl;
        cout << "D. " << db_optD[index] << endl;
    }
   
    else {
        
        char ans = db_correct[index];
      
        char wrongOpts[4];  // Array to store wrong option letters (max 3 wrong + 1 safety)
        int wrongCount = 0; // Counter for how many wrong options we've found
        
        // Check each option - if it's not the correct answer, add it to wrongOpts
        if (ans != 'A') wrongOpts[wrongCount++] = 'A';  // wrongCount++ adds then increments
        if (ans != 'B') wrongOpts[wrongCount++] = 'B';
        if (ans != 'C') wrongOpts[wrongCount++] = 'C';
        if (ans != 'D') wrongOpts[wrongCount++] = 'D';
       
        char keepDistractor = wrongOpts[rand() % wrongCount];
        
        bool showA = (ans == 'A' || keepDistractor == 'A');
        bool showB = (ans == 'B' || keepDistractor == 'B');
        bool showC = (ans == 'C' || keepDistractor == 'C');
        bool showD = (ans == 'D' || keepDistractor == 'D');
        
        // TERNARY OPERATOR: (condition ? valueIfTrue : valueIfFalse)
        // If show is true, print the option text; otherwise print "---"
        cout << "A. " << (showA ? db_optA[index] : "---") << endl;
        cout << "B. " << (showB ? db_optB[index] : "---") << endl;
        cout << "C. " << (showC ? db_optC[index] : "---") << endl;
        cout << "D. " << (showD ? db_optD[index] : "---") << endl;
    }
}

// playGame(): The main quiz game function - handles entire game session
// This is the most complex function with category selection, timer, lifelines, and scoring
void playGame() {
    // ========== SETUP PHASE ==========
    // Clear screen to start fresh
    system("cls");
    
    // DISPLAY CATEGORY SELECTION MENU
    cout << "========================================" << endl;
    cout << "          SELECT CATEGORY               " << endl;
    cout << "========================================" << endl;
    cout << "1. Science" << endl;
    cout << "2. Computer Science" << endl;
    cout << "3. Sports" << endl;
    cout << "4. History" << endl;
    cout << "5. IQ/Logic" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Enter Choice: ";
    
    // Read user's category choice
    int cat; 
    cin >> cat;
    string fname;
    switch(cat) {
        case 1: fname = "science.txt"; break;   // Science questions file
        case 2: fname = "computer.txt"; break;  // Computer Science questions file
        case 3: fname = "sports.txt"; break;    // Sports questions file
        case 4: fname = "history.txt"; break;   // History questions file
        case 5: fname = "iq.txt"; break;        // IQ/Logic questions file
        default: fname = "science.txt"; break;  // Default to science if invalid input
    }

    totalLoaded = loadData(fname);
    
    // ERROR CHECK: If no questions loaded, show error and return to menu
    if (totalLoaded == 0) {
        cout << "Error: No questions found!" << endl;
        myDelay(2);  // Wait 2 seconds so user can read the message
        return;      // Exit this function, go back to main menu
    }

    cout << "Choose Difficulty (1=Easy, 2=Med, 3=Hard)\n> ";
    int userDiff; 
    cin >> userDiff;
    
    string diffName = (userDiff == 1) ? "Easy" : (userDiff == 2) ? "Medium" : "Hard";

    int qIndices[MAX_Q];  // Array to hold indices 0, 1, 2, 3...
    
    // Fill array with sequential numbers: 0, 1, 2, 3, ... totalLoaded-1
    for (int i = 0; i < totalLoaded; i++) 
        qIndices[i] = i;
    
    // Shuffle the indices so questions appear in random order
    shuffleArray(qIndices, totalLoaded);

    // ========== SESSION VARIABLES ==========
    // These track the player's progress throughout the quiz
    
    int score = 0;           // Player's total score (can go negative with wrong answers)
    int streak = 0;          // Consecutive correct answers (resets on wrong answer)
    int qCount = 0;          // How many questions have been answered (max 10)
    int correctCount = 0;    // Total correct answers (for logging)
    int incorrectCount = 0;  // Total incorrect/timeout answers (for logging)

    int wrongIdx[10];        // Stores which question indices were answered wrong
    char wrongAns[10];       // Stores what the player answered (or 'T' for timeout)
    int wrongTotal = 0;      // How many wrong answers stored

    bool used50 = false;     // 50/50 lifeline: hides 2 wrong options
    bool usedSkip = false;   // Skip lifeline: skip question without penalty
    bool usedSwap = false;   // Swap lifeline: replace current question with different one
    bool usedTime = false;   // Extra Time lifeline: get 10 more seconds
    
    int i = 0;  // Iterator to go through shuffled indices
    while (qCount < 10 && i < totalLoaded) {
        // Get the actual question index from our shuffled array
        int idx = qIndices[i];
        if (db_diff[idx] != userDiff) {
            i++;       // Move to next question in shuffled list
            continue;  // Skip rest of loop, start next iteration
        }

        // Clear screen for each new question
        system("cls");
        
        // DISPLAY STATUS BAR:
        // Shows current score, streak, and question number
        cout << "Score: " << score << " | Streak: " << streak << " | Q: " << (qCount + 1) << "/10" << endl;

        // DISPLAY AVAILABLE LIFELINES:
        // Only show lifelines that haven't been used yet
        cout << "Lifelines: ";
        if (!used50) cout << "[F] 50/50  ";        // F key for Fifty-fifty
        if (!usedSkip) cout << "[S] Skip  ";       // S key for Skip
        if (!usedSwap) cout << "[W] Swap  ";       // W key for sWap
        if (!usedTime) cout << "[E] Extra Time";  // E key for Extra time
        cout << endl;

        // Display the current question with all 4 options
        printQuestion(idx, false);  // false = don't hide any options

        // Set time limit (normally 10 seconds)
        int timeLimit = 10;
        if (usedTime) timeLimit = 20;  // If time extension was used, show 20

        cout << "\n(You have " << timeLimit << "s) Enter Answer (A-D) or Lifeline Key: ";

        // ========== TIMER IMPLEMENTATION ==========
        // Record time when question was displayed
        time_t t1 = time(0);  // t1 = start time in seconds

        // Get player's input (answer or lifeline key)
        char input;
        cin >> input;
        
        // Convert to uppercase so 'a' and 'A' both work
        input = toupper(input);

        // Record time when answer was submitted
        time_t t2 = time(0);  // t2 = end time in seconds
        
        // Calculate how long the player took
        int timeTaken = (int)(t2 - t1);  // Difference in seconds
        int timeAllowed = 10;  // Default time limit

        // Variable to track if we need to get another answer (used by some lifelines)
        bool reAnswer = false;

        // ========== LIFELINE PROCESSING ==========
        // Check if input is a lifeline key and process it

        // LIFELINE 1: EXTRA TIME (Key: E)
        // Gives player 10 extra seconds to answer
        if (input == 'E' && !usedTime) {
            // Can only activate if within original time limit
            if (timeTaken <= 10) {
                usedTime = true;      // Mark lifeline as used
                timeAllowed = 20;     // Total time is now 20 seconds
                cout << ">> TIME EXTENDED! You have 20s total. Enter Answer: ";
                
                // Get the actual answer now
                cin >> input;
                input = toupper(input);
                
                // Recalculate total time taken (from original start)
                timeTaken = (int)(time(0) - t1);
            }
            else {
                cout << ">> Too late to use lifeline!" << endl;
            }
        }

        // LIFELINE 2: 50/50 (Key: F)
        // Hides 2 wrong options, leaving correct answer and 1 distractor
        if (input == 'F' && !used50) {
            if (timeTaken <= 10) {
                used50 = true;  // Mark lifeline as used
                system("cls");  // Clear screen to redisplay question
                cout << ">> 50/50 APPLIED!" << endl;
                
                // Redisplay question with 2 options hidden
                printQuestion(idx, true);  // true = hide 2 wrong options
                cout << "Enter Answer: ";

                // Reset timer - player gets fresh 10 seconds after 50/50
                t1 = time(0);
                cin >> input;
                input = toupper(input);
                timeAllowed = 10;
                timeTaken = (int)(time(0) - t1);
            }
            else {
                cout << ">> Too late to use lifeline!" << endl;
            }
        }

        // LIFELINE 3: SKIP (Key: S)
        // Skip current question without any penalty
        if (input == 'S' && !usedSkip) {
            usedSkip = true;  // Mark lifeline as used
            cout << ">> SKIPPED!" << endl;
            myDelay(1);  // Brief pause
            
            i++;       // Move to next question
            qCount++;  // Count this as an asked question (but no score change)
            continue;  // Skip to next loop iteration
        }

        // LIFELINE 4: SWAP (Key: W)
        // Replace current question with a different one
        if (input == 'W' && !usedSwap) {
            usedSwap = true;  // Mark lifeline as used
            cout << ">> SWAPPING QUESTION..." << endl;
            myDelay(1);
            
            i++;  // Move to next question in queue
            // NOTE: qCount is NOT incremented - the swapped question doesn't count
            continue;  // Skip to next loop iteration (will show different question)
        }

        // ========== CHECK IF TIME EXPIRED ==========
        // If player took longer than allowed time
        if (timeTaken > timeAllowed) {
            cout << "\n>> TIME UP! (" << timeTaken << "s taken) - Question marked as UNANSWERED" << endl;
            
            streak = 0;           // Reset streak on timeout
            incorrectCount++;     // Count timeout as incorrect for stats
            
            // Note: No negative marking for timeout - only marked as unanswered
            // (This follows the project specification)

            // SAVE FOR REVIEW: Store this question for end-of-game review
            wrongIdx[wrongTotal] = idx;   // Save which question it was
            wrongAns[wrongTotal] = 'T';   // 'T' = Timeout (didn't answer)
            wrongTotal++;                 // Increment wrong answer counter
        }
        // ========== CHECK IF ANSWER IS CORRECT ==========
        else {
            // Compare player's input with the correct answer stored in db_correct
            if (input == db_correct[idx]) {
                // CORRECT ANSWER!
                cout << "\n>> CORRECT!" << endl;
                
                streak++;         // Increment consecutive correct streak
                correctCount++;   // Increment total correct for stats
                
                // STREAK BONUS CALCULATION:
                // Players get bonus points for answering multiple questions correctly in a row
                int bonus = 0;
                if (streak == 3) { 
                    cout << "Streak Bonus +5!" << endl; 
                    bonus = 5;   // 3 in a row = +5 bonus
                }
                if (streak == 5) { 
                    cout << "Streak Bonus +15!" << endl; 
                    bonus = 15;  // 5 in a row = +15 bonus
                }
                
                // Add points: 10 base points + any streak bonus
                score += (10 + bonus);
            }
            else {
                // WRONG ANSWER!
                cout << "\n>> WRONG! Correct was: " << db_correct[idx] << endl;
                
                streak = 0;       // Reset streak - consecutive run broken
                incorrectCount++; // Increment total incorrect for stats
                
                // NEGATIVE MARKING:
                // Penalty depends on difficulty level chosen
                // Harder difficulty = bigger penalty for wrong answers
                if (userDiff == 1) 
                    score -= 2;   // Easy: -2 points
                else if (userDiff == 2) 
                    score -= 3;   // Medium: -3 points
                else 
                    score -= 5;   // Hard: -5 points

                // SAVE FOR REVIEW: Store this question for end-of-game review
                wrongIdx[wrongTotal] = idx;     // Save which question
                wrongAns[wrongTotal] = input;   // Save what player answered
                wrongTotal++;                   // Increment counter
            }
        }

        // Wait 2 seconds so player can see feedback before next question
        myDelay(2);
        
        // Move to next question
        qCount++;  // Increment questions answered count
        i++;       // Move to next index in shuffled array
    }

    // ========== END OF GAME - RESULTS SCREEN ==========
    system("cls");  // Clear screen for final results
    
    cout << "=== QUIZ COMPLETED ===" << endl;
    cout << "Final Score: " << score << endl;
    
    // Get player's name for the leaderboard
    cout << "Enter Name for Record: ";
    string name; 
    cin >> name;

    // Save score to leaderboard file and log the session
    updateLeaderboard(name, score, diffName);  // Add to high_scores.txt if qualifies
    logSession(name, score, qCount, correctCount, incorrectCount);  // Log to quiz_logs.txt

    // ========== REVIEW MODE ==========
    // If player got any questions wrong, offer to review them
    if (wrongTotal > 0) {
        cout << "\nReview wrong answers? (y/n): ";
        char r; 
        cin >> r;
        
        // Check if player wants to review (accepts 'y' or 'Y')
        if (r == 'y' || r == 'Y') {
            system("cls");  // Clear screen for review
            cout << "--- REVIEW ---" << endl;
            
            // Loop through all wrong answers
            for (int k = 0; k < wrongTotal; k++) {
                int originalIdx = wrongIdx[k];  // Get the question index
                
                // Display the question
                cout << "Q: " << db_question[originalIdx] << endl;
                
                // Display what the player answered
                // If 'T', it means they ran out of time
                cout << "Your Answer: " << wrongAns[k] << endl;
                
                // Display the correct answer
                cout << "Correct: " << db_correct[originalIdx] << endl << endl;
            }
            
            // Wait for player to press a key before returning to menu
            cout << "Press Key+Enter to exit...";
            string dummy; 
            cin >> dummy;
        }
    }
}

// ==========================================
// 6. FILE & DATA MGMT
// ==========================================

// Updates high_scores.txt with new entry, keeps top 5 sorted by score
// Format: Name | Date | Score | Difficulty
void updateLeaderboard(string name, int score, string diff) {
    // Temp Arrays for Sorting (parallel arrays for each column)
    string n[100], d[100], dates[100];  // Names, Difficulties, Dates
    int s[100];  // Scores
    int c = 0;

    // Read existing records
    ifstream fin("high_scores.txt");
    if (fin) {
        string line;
        while (getline(fin, line) && c < 100) {
            // Parse: Name|Date|Score|Difficulty
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            size_t pos3 = line.find('|', pos2 + 1);
            if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
                n[c] = line.substr(0, pos1);
                dates[c] = line.substr(pos1 + 1, pos2 - pos1 - 1);
                s[c] = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
                d[c] = line.substr(pos3 + 1);
                c++;
            }
        }
        fin.close();
    }

    // Add current entry with date
    n[c] = name;
    dates[c] = getCurrentDateTime();
    s[c] = score;
    d[c] = diff;
    c++;

    // Bubble Sort (Descending by score)
    for (int i = 0; i < c - 1; i++) {
        for (int j = 0; j < c - i - 1; j++) {
            if (s[j] < s[j + 1]) {
                swap(s[j], s[j + 1]);
                swap(n[j], n[j + 1]);
                swap(d[j], d[j + 1]);
                swap(dates[j], dates[j + 1]);
            }
        }
    }

    // Write Top 5 in format: Name|Date|Score|Difficulty
    ofstream fout("high_scores.txt");
    int limit = (c < 5) ? c : 5;
    for (int i = 0; i < limit; i++) {
        fout << n[i] << "|" << dates[i] << "|" << s[i] << "|" << d[i] << endl;
    }
    fout.close();
}

// Displays the leaderboard (top 5 high scores) from file
void viewLeaderboard() {
    system("cls");
    cout << "========================================" << endl;
    cout << "           LEADERBOARD (Top 5)          " << endl;
    cout << "========================================" << endl;
    
    ifstream fin("high_scores.txt");
    if (!fin) {
        cout << "No records found." << endl;
    }
    else {
        cout << "Rank | Name       | Date                    | Score | Difficulty" << endl;
        cout << "-----|------------|-------------------------|-------|------------" << endl;
        
        string line;
        int rank = 1;
        while (getline(fin, line)) {
            // Parse: Name|Date|Score|Difficulty
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            size_t pos3 = line.find('|', pos2 + 1);
            
            if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
                string name = line.substr(0, pos1);
                string date = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string score = line.substr(pos2 + 1, pos3 - pos2 - 1);
                string diff = line.substr(pos3 + 1);
                
                cout << "  " << rank << "  | " << name;
                // Padding for alignment
                for (size_t i = name.length(); i < 10; i++) cout << " ";
                cout << " | " << date << " | " << score;
                for (size_t i = score.length(); i < 5; i++) cout << " ";
                cout << " | " << diff << endl;
                rank++;
            }
        }
        fin.close();
    }
    cout << "\nPress 1 and Enter to return: ";
    int dum; cin >> dum;
}

// Logs complete session details to quiz_logs.txt in append mode
// Includes: Timestamp, User, Total Attempted, Correct, Incorrect, Final Score
void logSession(string name, int score, int totalQ, int correctCount, int incorrectCount) {
    ofstream fout("quiz_logs.txt", ios::app);
    string dt = getCurrentDateTime();
    
    fout << "========================================" << endl;
    fout << "Session Log: [" << dt << "]" << endl;
    fout << "User: " << name << endl;
    fout << "Total Attempted: " << totalQ << "/10" << endl;
    fout << "Correct Answers: " << correctCount << endl;
    fout << "Incorrect/Unanswered: " << incorrectCount << endl;
    fout << "Final Score: " << score << endl;
    fout << "========================================" << endl << endl;
    fout.close();
}

// Loads question data from a text file into parallel arrays
// File Format: Difficulty, Question, OptA, OptB, OptC, OptD, CorrectAnswer
// Returns the number of questions successfully loaded
int loadData(string filename) {
    ifstream fin(filename);
    if (!fin) return 0;

    int count = 0;
    while (fin >> db_diff[count] && count < MAX_Q) {
        fin.ignore(); // Skip newline after difficulty number
        getline(fin, db_question[count]);
        getline(fin, db_optA[count]);
        getline(fin, db_optB[count]);
        getline(fin, db_optC[count]);
        getline(fin, db_optD[count]);
        fin >> db_correct[count];
        fin.ignore(); // Skip newline after correct answer
        count++;
    }
    fin.close();
    return count;
}

// Generates default question bank files if they don't exist
// Creates files for: Science, Computer Science, Sports, History, IQ/Logic
void generateFiles() {
    // Generate Science Questions
    ifstream f1("science.txt");
    if (!f1.good()) {
        ofstream out("science.txt");
        out << "1\nChem symbol for Water?\nH2O\nCO2\nO2\nN\nA\n";
        out << "2\nRed Planet?\nVenus\nMars\nJupiter\nSaturn\nB\n";
        out << "1\nFreezing point of water?\n0C\n100C\n-10C\n50C\nA\n";
        out << "3\nSpeed of Light?\n300k km/s\n150k km/s\nSound speed\nUnknown\nA\n";
        out << "1\nHardest Natural Substance?\nGold\nIron\nDiamond\nSilver\nC\n";
        out << "2\nHuman Heart has how many chambers?\n2\n3\n4\n5\nC\n";
        out << "3\nAtomic Number of O?\n6\n7\n8\n9\nC\n";
        out << "1\nWhat do bees make?\nMilk\nHoney\nSilk\nCotton\nB\n";
        out << "2\nLargest Mammal?\nElephant\nBlue Whale\nShark\nGiraffe\nB\n";
        out << "1\nBones in adult human?\n206\n200\n300\n210\nA\n";
        out.close();
    }
    f1.close();

    // Generate Computer Science Questions
    ifstream f2("computer.txt");
    if (!f2.good()) {
        ofstream out("computer.txt");
        out << "1\nBrain of Computer?\nRAM\nCPU\nHDD\nMouse\nB\n";
        out << "1\nInput Device?\nMonitor\nPrinter\nMouse\nSpeaker\nC\n";
        out << "2\nStandard web markup language?\nHTML\nC++\nPython\nJava\nA\n";
        out << "3\nBinary for 5?\n100\n101\n110\n111\nB\n";
        out << "1\nRAM stands for?\nRead Access Memory\nRandom Access Memory\nRun All Memory\nNone\nB\n";
        out << "2\n1 Byte = ? bits\n4\n8\n16\n32\nB\n";
        out << "3\nHTTP port?\n21\n25\n80\n443\nC\n";
        out << "1\nWindows is a...?\nOS\nBrowser\nEditor\nGame\nA\n";
        out << "2\nFounder of Microsoft?\nJobs\nGates\nMusk\nBezos\nB\n";
        out << "3\nBig O of Linear Search?\nO(1)\nO(n)\nO(log n)\nO(n^2)\nB\n";
        out.close();
    }
    f2.close();

    // Generate Sports Questions
    ifstream f3("sports.txt");
    if (!f3.good()) {
        ofstream out("sports.txt");
        out << "1\nHow many players in a football team?\n10\n11\n12\n9\nB\n";
        out << "1\nOlympics held every ? years\n2\n3\n4\n5\nC\n";
        out << "2\nWho has most Ballon d'Or?\nRonaldo\nMessi\nNeymar\nMbappe\nB\n";
        out << "2\nCricket World Cup 2019 winner?\nIndia\nAustralia\nEngland\nPakistan\nC\n";
        out << "3\nFastest 100m sprint record holder?\nBolt\nGatlin\nPowell\nBlake\nA\n";
        out << "1\nTennis Grand Slams per year?\n2\n3\n4\n5\nC\n";
        out << "2\nNBA team with most championships?\nLakers\nCeltics\nBulls\nWarriors\nB\n";
        out << "3\nFirst FIFA World Cup year?\n1930\n1934\n1950\n1966\nA\n";
        out << "1\nHockey is national sport of?\nIndia\nPakistan\nBoth\nNeither\nC\n";
        out << "2\nF1 races are called?\nMatches\nGrand Prix\nDerbies\nSprints\nB\n";
        out.close();
    }
    f3.close();

    // Generate History Questions
    ifstream f4("history.txt");
    if (!f4.good()) {
        ofstream out("history.txt");
        out << "1\nWho discovered America?\nColumbus\nCook\nMagellan\nDrake\nA\n";
        out << "2\nWorld War 2 ended in?\n1943\n1944\n1945\n1946\nC\n";
        out << "1\nFirst man on moon?\nBuzz\nArmstrong\nGlenn\nShepard\nB\n";
        out << "3\nFrench Revolution year?\n1776\n1789\n1799\n1815\nB\n";
        out << "2\nBerlin Wall fell in?\n1987\n1988\n1989\n1990\nC\n";
        out << "1\nEgyptian writing system?\nCuneiform\nHieroglyphics\nSanskrit\nLatin\nB\n";
        out << "3\nMagna Carta signed in?\n1066\n1215\n1415\n1492\nB\n";
        out << "2\nWho built the Taj Mahal?\nAkbar\nShah Jahan\nJahangir\nAurangzeb\nB\n";
        out << "1\nGreat Wall of China built by?\nMing\nQin\nHan\nTang\nB\n";
        out << "3\nRoman Empire fell in?\n376 AD\n410 AD\n476 AD\n500 AD\nC\n";
        out.close();
    }
    f4.close();

    // Generate IQ/Logic Questions
    ifstream f5("iq.txt");
    if (!f5.good()) {
        ofstream out("iq.txt");
        out << "1\nWhat comes next: 2,4,6,8,?\n9\n10\n11\n12\nB\n";
        out << "2\nIf 5+3=28, 9+1=810, then 2+6=?\n212\n812\n28\n68\nA\n";
        out << "1\nHow many months have 28 days?\n1\n2\n6\n12\nD\n";
        out << "3\nNext: 1,1,2,3,5,8,?\n11\n12\n13\n14\nC\n";
        out << "2\nIf APPLE=50, then BANANA=?\n60\n62\n42\n55\nC\n";
        out << "1\nFather's brother is your?\nCousin\nNephew\nUncle\nBrother\nC\n";
        out << "2\nMirror of AMBULANCE reads?\nECNALUBMA\nReverse\nAMBULANCE\nNormal\nC\n";
        out << "3\nComplete: 3,6,11,18,?\n25\n27\n29\n31\nB\n";
        out << "1\nOdd one out: Cat,Dog,Car,Rat?\nCat\nDog\nCar\nRat\nC\n";
        out << "3\nIf RED=27, GREEN=49, BLUE=?\n36\n40\n42\n38\nC\n";
        out.close();
    }
    f5.close();
}