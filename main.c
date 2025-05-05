#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>

#include "cursor.h"
#include "game_data.h"
#include "safe_alloc.h"

#define PER_LINE_CHAR_LIMIT 40
#define USER_INPUT_ARRAY_SIZE 20
#define TIME_DISPLAY_REFRESH_RATE 10

char userPrefix;

void promptSafeUnderlinedPrint(const char* str, int lineCharsLeft, boolean leadingSpace) {
    if (leadingSpace) {
        lineCharsLeft -= strlen(str) + 1;
        if (lineCharsLeft > 0) {
            printf("\033[4m%s\033[24m ", str);
        }
        else {
            printf("\n  \033[4m%s\033[24m ", str);
            lineCharsLeft = PER_LINE_CHAR_LIMIT;
        }
    }
    else {
        lineCharsLeft -= strlen(str);
        if (lineCharsLeft > 0) {
            printf("\033[4m%s\033[24m", str);
        }
        else {
            printf("\n  \033[4m%s\033[24m", str);
            lineCharsLeft = PER_LINE_CHAR_LIMIT;
        }
    }
}

void promptSafeProgressPrint(const char* str, int* lineCharsLeft, boolean leadingSpace) {
    if (leadingSpace) {
        *lineCharsLeft -= strlen(str) + 1;
        if (*lineCharsLeft > 0) {
            printf("%s ", str);
        }
        else {
            printf("\n  %s ", str);
            *lineCharsLeft = PER_LINE_CHAR_LIMIT;
        }
    }
    else {
        *lineCharsLeft -= strlen(str);
        if (*lineCharsLeft > 0) {
            printf("%s", str);
        }
        else {
            printf("\n  %s", str);
            *lineCharsLeft = PER_LINE_CHAR_LIMIT;
        }
    }
}

void printTextPromptByWord(char* wordArray[], int totalWords) {
    int lineCharsLeft = PER_LINE_CHAR_LIMIT;
    printf("\033[K"); // Clear initial line
    for (int i = 0; i < totalWords; i++) {
        if (i + 1 >= totalWords) { // If last word, don't count or print space
            lineCharsLeft -= strlen(wordArray[i]);
            if (lineCharsLeft > 0) {
                printf("%s", wordArray[i]);
            }
            else { // If not enough chars remaining on line for word, goto newline and clear the line before printing the word
                printf("\n\033[K  %s", wordArray[i]);
                lineCharsLeft = PER_LINE_CHAR_LIMIT;
            }
        }
        else {
            lineCharsLeft -= strlen(wordArray[i]) + 1; // +1 for space
            if (lineCharsLeft > 0) {
                printf("%s ", wordArray[i]);
            }
            else {
                printf("\n  %s ", wordArray[i]);
                lineCharsLeft = PER_LINE_CHAR_LIMIT;
            }
        }
    }
}

void recursivePrint(const char* string, int i) {
    if (i > 0) {
        printf("%s", string);
        recursivePrint(string, i - 1);
    }
}

void initUserPrefix() {
    userPrefix = '>';
}

void changeUserPrefix(const char newPrefix) {
    strcpy_s(userPrefix, sizeof(userPrefix), newPrefix);
}

void formatTime(int total_seconds) {
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;

    if (hours > 0) {
        printf("%02d:%02d:%02d", hours, minutes, seconds);
    }
    else {
        printf("%02d:%02d", minutes, seconds);
    }
}

void updateTime(CursorPos pos, time_t start_time, time_t current_time, time_t time_limit) {
    // Save the current cursor position
    CursorPos previousPos;
    saveCursor(&previousPos);

    // Hide cursor and move it to the top-left
    printf("\033[?25l");
    moveCursor(pos);

    // Calculate total remaining seconds
    int tr_total_seconds = (long)time_limit - ((long)current_time - start_time);

    printf("Time Remaining  -  ");
    if (tr_total_seconds <= 10) { // Red text color when <= 30 seconds
        printf("\033[91m");
    }
    else if ((((double)tr_total_seconds / time_limit) * 100) <= 50) { // Yellow text color when <=50% time remaining
        printf("\033[33m");
    }
    formatTime(tr_total_seconds); // Print formatted time (hh:/mm:ss) using total remaining seconds
    printf("\033[0m\033[K"); // Return to normal text color and clear remaining parts of the line after cursor

    // Move cursor to the saved position and unhide it
    moveCursor(previousPos);
    printf("\033[?25h");

    // Immediately update console output from buffer
    fflush(stdout);
}

int main() {

    initUserPrefix();

    GamemodeData* selectedGamemode;
    LevelData* currentLevel;
    PromptData* userPrompt;

    // Initialize cursor positions
    CursorPos homeCursor = createCursor(1, 1); // top-left of console window
    CursorPos promptStart;
    CursorPos currentPromptProgress;
    CursorPos userInputField;

    // Create and organize prompt data
    GamemodeData g1 = createGamemode();

    LevelData l1 = createLevel();
    addPromptStrToLevel(&l1, "First prompt");
    addPromptStrToLevel(&l1, "Second prompt");
    addPromptStrToLevel(&l1, "Third prompt");
    LevelData l2 = createLevel();
    addPromptStrToLevel(&l2, "Fourth prompt");
    addPromptStrToLevel(&l2, "Fifth prompt");
    addPromptStrToLevel(&l2, "Sixth prompt");
    LevelData l3 = createLevel();
    addPromptStrToLevel(&l3, "Seventh prompt");
    addPromptStrToLevel(&l3, "Eighth prompt");
    addPromptStrToLevel(&l3, "Ninth prompt");

    addLevelToGamemode(&g1, l1);
    addLevelToGamemode(&g1, l2);
    addLevelToGamemode(&g1, l3);

    GamemodeData g2 = createGamemode();

    LevelData g2l1 = createLevel();
    	addPromptStrToLevel(&g2l1, "First prompt");
	addPromptStrToLevel(&g2l1, "Second prompt");
	addPromptStrToLevel(&g2l1, "Third prompt");
    LevelData g2l2 = createLevel();
	addPromptStrToLevel(&g2l2, "Fourth prompt");
	addPromptStrToLevel(&g2l2, "Fifth prompt");
	addPromptStrToLevel(&g2l2, "Sixth prompt");
    LevelData g2l3 = createLevel();
	addPromptStrToLevel(&g2l3, "Seventh prompt");
	addPromptStrToLevel(&g2l3, "Eigth prompt");
	addPromptStrToLevel(&g2l3, "Ninth prompt");
    LevelData g2l4 = createLevel();
	addPromptStrToLevel(&g2l4, "Tenth prompt");
	addPromptStrToLevel(&g2l4, "Eleventh prompt");
	addPromptStrToLevel(&g2l4, "Twelfth prompt");

	addLevelToGamemode(&g2, g2l1);
    	addLevelToGamemode(&g2, g2l2);
    	addLevelToGamemode(&g2, g2l3);
	addLevelToGamemode(&g2, g2l4);

     GamemodeData g3 = createGamemode();

     LevelData g3l1 = createLevel();
	addPromptStrToLevel(&g3l1, "You shuddered as you gazed, and wondered what monstrous cannibal
		and savage could ever have gone a death-harvesting with such a hacking, horrifying implement. 
		Mixed with these were rusty old whaling lances and harpoons all broken and deformed.");
	addPromptStrToLevel(&g3l1, "Now, that Lazarus should lie stranded there on the curbstone before the door of Dives, 
		this is more wonderful than that an iceberg should be moored to one of the Moluccas.");
	addPromptStrToLevel(&g3l1, "Let them talk of their oriental summer climes of everlasting conservatories; 
		give me the privilege of making my own summer with my own coals.");
	addPromptStrToLevel(&g3l1, "But it’s too late to make any improvements now. 
		The universe is finished; the copestone is on, and the chips were carted off a million years ago.");
     LevelData g3l2 = createLevel();
	addPromptStrToLevel(&g3l2, "It had a careless look, as if it were meant for the uses of the public; so, entering, 
		the first thing I did was to stumble over an ash-box in the porch.");
	addPromptStrToLevel(&g3l2, "Now having a night, a day, and still another night following before me in New Bedford, 
		ere I could embark for my destined port, it became a matter of concernment where I was to eat and sleep meanwhile.");
	addPromptStrToLevel(&g3l2, "As most young candidates for the pains and penalties of whaling stop at this same New Bedford, 
		thence to embark on their voyage, it may as well be related that I, for one, had no idea of so doing.");
	addPromptStrToLevel(&g3l2, "He thinks he breathes it first; but not so. 
		In much the same way do the commonalty lead their leaders in many other things, at the same time that the leaders little suspect it.");
     LevelData g3l3 = createLevel; 
	addPromptStrToLevel(&g3l3, "And there is all the difference in the world between paying and being paid. 
		The act of paying is perhaps the most uncomfortable infliction that the two orchard thieves entailed upon us.");
	addPromptStrToLevel(&g3l3, "Deep into distant woodlands winds a mazy way, 
		reaching to overlapping spurs of mountains bathed in their hill-side blue.");
	addPromptStrToLevel(&g3l3, "Should you ever be athirst in the great American desert, try this experiment,
		if your caravan happen to be supplied with a metaphysical professor. Yes, as every one knows, meditation and water are wedded for ever.");
	addPromptStrToLevel(&g3l3, "Say, you are in the country; in some high land of lakes. 
		Take almost any path you please, and ten to one it carries you down in a dale, 
		and leaves you there by a pool in the stream. there is magic in it.");

	addLevelToGamemode(&g3, g3l1);
    	addLevelToGamemode(&g3, g3l2);
    	addLevelToGamemode(&g3, g3l3);

      GamemodeData g4 = createGamemode();

      LevelData g4l1 = createLevel();
	addPromptStrToLevel(&g4l1, "Prince Vasili did not reply, though, with the 
		quickness of memory and perception befitting a man of the world,
		he indicated by a movement of the head that he was considering this information.");
	addPromptStrToLevel(&g4l1, "Anna Pavlovna’s alarm was justified, 
		for Pierre turned away from the aunt without waiting to hear her speech about Her Majesty’s health.");
      LevelData g4l2 = createLevel();
	addPromptStrToLevel(&g4l2, "Pierre murmured something unintelligible, and continued to look round as if in search of something. 
		On his way to the aunt he bowed to the little princess with a pleased smile, as to an intimate acquaintance.");
	addPromptStrToLevel(&g4l2, "Though he was certainly rather bigger than the other men in the room, 
		her anxiety could only have reference to the clever though shy, 
		but observant and natural, expression which distinguished him from everyone else in that drawing room.");
      LevelData g4l3 = createLevel();
	addPromptStrToLevel(&g4l3, "The young man had not yet entered either the military or civil service, 
		as he had only just returned from abroad where he had been educated, 
		and this was his first appearance in society. ");
	addPromptStrToLevel(&g4l3, " All who talked to her, and at each word saw her bright smile and the constant gleam of her white teeth, 
		thought that they were in a specially amiable mood that day.");
      LevelData g4l4 = createLevel();
	addPromptStrToLevel(&g4l4, "Her pretty little upper lip, on which a delicate dark down was just perceptible,
		was too short for her teeth, but it lifted all the more sweetly, 
		and was especially charming when she occasionally drew it down to meet the lower lip.");
	addPromptStrToLevel(&g4l4, "Each visitor performed the ceremony of greeting this old aunt whom not one of them knew, 
		not one of them wanted to know, and not one of them cared about; 
		Anna Pavlovna observed these greetings with mournful and solemn interest and silent approval.");
      LevelData g4l5 = createLevel();
	addPromptStrToLevel(&g4l5, "The highest Petersburg society was assembled there: people differing widely in age and character but alike in the social circle to which they belonged. 
		Prince Vasili’s daughter, the beautiful Helene, came to take her father to the ambassador’s entertainment; 
		she wore a ball dress and her badge as maid of honor.");
	addPromptStrToLevel(&g4l5, "Her father is very rich and stingy. He lives in the country.
		He is the well-known Prince Bolkonski who had to retire from the army under the late Emperor, 
		and was nicknamed the King of Prussia. He is very clever but eccentric, and a bore. The poor girl is very unhappy. ");

	addLevelToGamemode(&g4, g4l1);
    	addLevelToGamemode(&g4, g4l2);
    	addLevelToGamemode(&g4, g4l3);
	addLevelToGamemode(&g4, g4l4);
    	addLevelToGamemode(&g4, g4l5);

    char userResponse[USER_INPUT_ARRAY_SIZE];

	char ch;
	int time_limit = 40;
    int updateTimeLoop = 0;

    // Initialize time
    time_t start_time;
    time_t current_time;

    // Disable cursor blinking
    printf("\033[?12l");

    // Set loop variables
    int wordIndex;
    int promptIndex;
    int levelIndex = 0;
    int totalPromptChars = 0;
    int totalInputErrors = 0;
    int wordLength;
    int wordErrors;
    int wordInputIndex;
    boolean charLoop;
    boolean wordLoop;
    boolean promptLoop;
    boolean levelLoop;
    boolean playLoop;
    boolean gameLoop;
    int lineCharsLeft;
    int winState = -1;

    // Hide cursor
    printf("\033[?25l");

    levelLoop = true;

    selectedGamemode = &g1;

    // Cycle between the levels in the selected gamemode
    while (levelLoop) {
        // Move cursor to the top-left of the console window
        moveCursor(homeCursor);
        // Display timer on current line
        start_time = time(NULL);
        current_time = time(NULL);
        updateTime(homeCursor, start_time, current_time, time_limit);
        // Set current level to specified index of selected gamemode
        currentLevel = &selectedGamemode->levelArray[levelIndex];
        // Display level info on next line
        printf("\nLevel %d/%d\033[K", levelIndex + 1, selectedGamemode->totalLevels);
        // Navigate to where the prompt will be printed
        printf("\n\n  ");
        // Save current cursor position to promptStart
        saveCursor(&promptStart);

        // Reset prompt index
        promptIndex = 0;
        promptLoop = true;

        // Cycle between the prompts in each level
        while (promptLoop && levelLoop) {
            // Move cursor to the start of the prompt
            moveCursor(promptStart);
            // Set current prompt progress cursor to prompt start cursor
            currentPromptProgress = promptStart;
            // Clear all text from the screen past the cursor
            printf("\033[0J");

            // Set userPrompt to prompt at specified index of current level
            userPrompt = &currentLevel->promptArray[promptIndex];
            // Procedurally print prompt word by word with newlines when line char limit is reached
            printTextPromptByWord(userPrompt->wordArray, userPrompt->totalWords);
            // Create input field and save it's position to a cursor
            printf("\n\n%c ", userPrefix);
            saveCursor(&userInputField);

            // Reset line chars left
            lineCharsLeft = PER_LINE_CHAR_LIMIT;
            // Reset word index
            wordIndex = 0;

            // Move cursor back to the start of the prompt
            moveCursor(promptStart);
            // Hide the cursor
            printf("\033[?25l");

            wordLoop = true;

            // Cycle between the words in each prompt
            while (wordLoop && promptLoop && levelLoop) {
                // Update word length to size of current word at index
                wordLength = strlen(userPrompt->wordArray[wordIndex]);
                // Set all elements of the userResponse array to the null character
                memset(userResponse, '\0', sizeof(userResponse));

                // Print underlined version of word at current index
                promptSafeUnderlinedPrint(userPrompt->wordArray[wordIndex], lineCharsLeft, true);
                // Move cursor back to start of input field
                moveCursor(userInputField);
                // Unhide cursor
                printf("\x1B[?25h");

                totalPromptChars += wordLength;
                wordErrors = 0;
                wordInputIndex = 0;

                charLoop = true;

                // Cycle between the characters in each word
                while (charLoop && wordLoop && promptLoop && levelLoop) {
                    while (!_kbhit() && levelLoop) {  // Wait for a key press without blocking
                        updateTimeLoop++; // Increment time display update loop count
                        current_time = time(NULL); // Update current time
                        if (current_time - start_time >= time_limit) { // Fail user if they exceeded the time limit
                            winState = 0;
                            levelLoop = false;
                        }
                        if (updateTimeLoop >= TIME_DISPLAY_REFRESH_RATE) { // Update time display if TDU loop count exceeds threshold
                            updateTimeLoop = 0;
                            updateTime(homeCursor, start_time, current_time, time_limit);
                        }
                        Sleep(1);
                    }
                    if (charLoop && wordLoop && promptLoop && levelLoop) {
                        ch = _getch();  // Read input once a key is pressed
                        if (ch == 8) { // If backspace
                            if (wordInputIndex > 0) { // If not at beginning of input
                                wordInputIndex--;
                                printf("%c \033[D", ch); // Overwrite character behind cursor with a space
                            }
                        }
                        else if (ch == -32) { // If arrow keys
                            ch = _getch(); // Block input of second arrow key character
                        }
                        else if (!(ch == 10 || ch == 13)) { // If not \n or \r (enter)
                            if (wordInputIndex == wordLength && ch == 32) { // If user at final index of prompt and presses space
                                //int result = strcmp(userPrompt, userResponse);
                                //printf("\n\n%s\n%s\n%d", userPrompt, userResponse, result);
                                if (strcmp(userPrompt->wordArray[wordIndex], userResponse) == 0) {
                                    charLoop = false;
                                }
                            }
                            if (wordInputIndex < wordLength) { // If user not at final index of prompt
                                userResponse[wordInputIndex] = ch; // Update char array that tracks input
                                if (ch == userPrompt->wordArray[wordIndex][wordInputIndex]) { // If input was correct, print normally
                                    printf("%c", ch);
                                }
                                else { // If input was incorrect, print character in light red text
                                    printf("\033[31m%c\x1B[0m", ch);
                                    wordErrors++;
                                }
                                wordInputIndex++;
                            }
                        }
                    }
                }

                // Hide cursor
                printf("\033[?25l");
                // Move cursor to start of input field
                moveCursor(userInputField);
                // Erase line after cursor
                printf("\033[K");

                // Print prompt progress if word loop is still active
                if (wordLoop && promptLoop && levelLoop) {
                    // Move cursor to where the progress printer left off
                    moveCursor(currentPromptProgress);

                    // Print light green / orange version of the word at the current index
                    if (wordErrors > 0) {
                        printf("\033[38;5;208m");
                    }
                    else {
                        printf("\033[92m");
                    }
                    promptSafeProgressPrint(userPrompt->wordArray[wordIndex], &lineCharsLeft, true);
                    printf("\033[0m");
                    // Save progress printer position
                    saveCursor(&currentPromptProgress);

                    totalInputErrors += wordErrors;
                    wordIndex++;

                    if (wordIndex >= userPrompt->totalWords) { // If loop is still enabled and user completed all the words
                        wordLoop = false;
                    }
                }
            }

            if (promptLoop && levelLoop) {
                promptIndex++;

                if (promptIndex >= currentLevel->totalPrompts) { // If loop is still enabled and user completed all the prompts
                    promptLoop = false;
                }
            }
        }

        if (levelLoop) {
            levelIndex++;

            if (levelIndex >= selectedGamemode->totalLevels) { // If loop is still enabled and user completed all the levels
                levelLoop = false;
            }
        }
    }

    // Move cursor to start of input field
    moveCursor(userInputField);

    if (winState == 0) {
        printf("\nTimes up! You lost.\n");
    }
    else {
        printf("\nYou won!\n");
    }

    Sleep(2000);

    printf("Press any key to continue...");
    _getch();

	return 0;
}
