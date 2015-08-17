# The Strategy
### 1. Word Length:
The AI starts with a list of all valid English words, then removes any words that aren't the right length. So if the game looked like this: _ _ _ , the program would remove words like "code", "um", and "histocompatibility", because none of them are three letters long.

### 2. Letter Frequencies:
When it comes time to make a guess, the AI looks at each letter in the alphabet and counts how many of the possible words contain that letter. For words with multiple of the same letter, the letter is only counted once per word; so for the word "tiki", the letter 'I' only counts once. Whichever letter appears in the most words gets picked by the AI as a guess.

### 3. Common Words:
But what if you have a bunch of guesses that are equally good? Like if the game looked like _ I T H, and letters like 'K', 'P', 'S' and 'W' have an equal shot at being the right letter? The AI sees this and turns to its list of the 80,000 most common words ordered by frequency in the English language, then looks at the letter frequencies for any possible words within the list of most common words. In this case, "with" is the only one out of the four possibilities that is on the common words list, so the list looks like K: 0, P: 0, S: 0 and W: 1. 'W' is the clear winner, so that's what AI guesses.


But what if you have letters that are equally as good for all words and for common words? Like if the game looked _ A T, and you could pick 'B', 'E', 'S' or a bunch of other letters that form words that are all on the common words list? What do you do then? Well remember that the list of common words is sorted by common-ness, so the AI just takes the average of the indices of the words each of letters can form, and pickes the lowest average. So in this case, "bat is 2782nd, "eat" is 342nd, etc. It turns out that 'E' is the best letter, so the AI guesses 'E'.

### 4. Letter Positions
The AI comes up with a guess, let's say the letter 'I', and it turns out that the letter isn't in the actual word. This isn't so bad, because now it can remove any word with an 'I' in it from its list of possible words, narrowing down the possibilities for another guess.

Now the AI guesses 'A', which turns out to be the first letter of the word. Now the game looks like this: A _ _, and we can get rid of any word that doesn't have 'A' as its first letter, like 'man', 'ton' and 'key'. You can even get rid of words like 'aha', because even though it's first letter is 'A', so is its last letter, and we know that the only place 'A' shows up is as the first letter of the word.

### 5. Previous Games
The AI manages to guess the word; let's say the word was 'arm'. It tries to learn from this by putting the word into the middle of the common words list, so that the more you play, the more the AI knows which words you usually go for and factors it in accordingly. However, in this case 'arm' is already in the list of common words, so instead it's moved up a few thousand places in the ranking.
