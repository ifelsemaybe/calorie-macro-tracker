# calorie-macro-tracker

This here program helps me track the caloric content of all the foods I eat, during my fitness training blocks, where I aim to bulk up. It is more a prototype if anything, yet it is very functional and I use it every day.

It works by reading the "Food Stats.txt" file, which acts as a database that stores two different entities, Ingredients and Meals. The main relationship between both these entities is that meals are composed of various different ingredients, whilst ingredients are solely, a single item. Both meals and ingredients contain Macros (proteins, carbohydrates, fats) and a Caloric value. I then proceed to track my day through three different "time periods" (Breakfast, Lunch, Dinner), whereby after I'm done with the dinner, the program can proceed to logging your day in the "log.txt" file.

This application handles a lot of the tedious ratio calculations behind nutrition tracking. An example would be, say you know 60 grams of an apple holds 100 calories, then how many calories would the apple weighing 213 grams hold that you just ate. You'd have to do 213/60 * 100 to get your answer. This app allows for tracking meals/ingredients volumetrically (ml, tsp, tbsp, cup), through weight (g), or through count (i.e. x3).

## How To Run

If on Windows (64 bit): look into the directory x64_Release and double-click Calorie_Macro_Tracker.exe. Else, you'd need to compile the source files with a c++ compiler of your choice.

(Project was built using Visual Studio 2022, on Windows, and coded entirely in C++).

## Walkthrough

![image 1](/img/image_1.png)

You are first greeted by this, where you can input 1 of 4 different commands: {input ingredient, input meal, track, debug}, or you can alternitevely leave by typing {e, exit}.

### 1. Input Ingredient

![image 2](/img/image_2.png)

Input ingredient writes a new ingredient to "Food Stats.txt", in the Ingredient section, through user input. Note, for the proportion, you can use all of the above (ml, tsp, tbsp, cup, g, x{number}) or type in n or null to leave it blank, which effectively acts the same way as if you'd have typed in "x1".

### 2. Input Meal

![image 3](/img/image_3.png)

Input meal writes a new meal to "Food Stats.txt", in the Meal section, again, through user input. It will prompt you to enter a list of ingredients and their proportions, and it is very important the ingredient inside the meal holds the same proportion type as the ingredient inside the Ingredient section of the file, otherwise unexpected behaviour and crashing can be expected.

### 3. Track

![image 4](/img/image_4.png)

Tracking is the meat and potatoes of this app. It will first ask you for whether you want to input an Ingredient or a Meal.

- Tracking Ingredient

![image 5](/img/image_5.png)

Again, the proportion type given must match the proportion type in the Ingredient section of "Food Stats.txt". Optionally you can also enter n or null, to have the ingredient be tracked with its default proportion, stated in "Food Stats.txt".

- Tracking Meal

![image 6](/img/image_6.png)

When tracking the meal, you have 2 options: 1-changing the meal proportion (cmp), and 2-changing ingredient proportion (cip). I mainly use the first option when you have a meal such as a stew or soup, where you know the total weight of your meal, and then you measure out the weight of one serving which you would input into the cmp. The second option is useful when wanting to change individual ingredients inside of your meal.

- Storing Data

![image 7](/img/image_7.png)

Once you've exited out of the tracking mode, the app will ask you where you want to store the values you've just tracked.
You have three options here: breakfast, lunch, dinner, or you can also input "e", to not store them anywhere. This uses serialization, so once they have been stored to a particular time period, even if the application is closed, the values will be remembered.

If you are tracking values into breakfast, for the first time, it will ask for your weight. If you are tracking values into dinner, it will ask if you want to log them, whereby if you do, all three time periods get reset to 0 items tracked, symbolizing the end of tracking for that particular day.

### 4. Debug

![image 8](/img/image_8.png)

This outputs to console, all the values read from "Food Stats.txt" and any newly stored values that have just gotten inputed through Input Ingredient or Input Meal. It also shows the total Macros and Calories stored in each Breakfast, Lunch and Dinner time periods, as well as showing the total of all three of those periods added up.

