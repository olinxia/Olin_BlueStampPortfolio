# Real Time Planetary Tracker
Replace this text with a brief description (2-3 sentences) of your project. This description should draw the reader in and make them interested in what you've built. You can include what the biggest challenges, takeaways, and triumphs from completing the project were. As you complete your portfolio, remember your audience is less familiar than you are with all that your project entails!

| **Engineer** | **School** | **Area of Interest** | **Grade** |
|:--:|:--:|:--:|:--:|
| Olin X | The College Preparatory School | Aerospace Engineering | Incoming Junior

<img src="OlinX.png" width="300" height="400">
  
<!--
# Final Milestone

**Don't forget to replace the text below with the embedding for your milestone video. Go to Youtube, click Share -> Embed, and copy and paste the code to replace what's below.**

<iframe width="560" height="315" src="https://www.youtube.com/embed/F7M7imOVGug" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>

For your final milestone, explain the outcome of your project. Key details to include are:
- What you've accomplished since your previous milestone
- What your biggest challenges and triumphs were at BSE
- A summary of key topics you learned about
- What you hope to learn in the future after everything you've learned at BSE



# Second Milestone

**Don't forget to replace the text below with the embedding for your milestone video. Go to Youtube, click Share -> Embed, and copy and paste the code to replace what's below.**

<iframe width="560" height="315" src="https://www.youtube.com/embed/y3VAmNlER5Y" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>

For your second milestone, explain what you've worked on since your previous milestone. You can highlight:
- Technical details of what you've accomplished and how they contribute to the final goal
- What has been surprising about the project so far
- Previous challenges you faced that you overcame
- What needs to be completed before your final milestone 

-->

# First Milestone

<iframe width="560" height="315" src="https://www.youtube.com/embed/phVcLO6DWw8?si=puXWAZY8hmOiKYs6" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

### Summary
My project, the real time planet tracker, will be able to track all the planets in the solar system and show me where they are in the sky. There are many components that have to work together to make my project work. Here is a quick summary:
  - Arduino board: stores all the code and communicates with all the other parts
  - GPS Module: records the time and location for the Arduino board to incorporate in its calculations
  - 

### Progress
For my first milestone, I focused on building the wiring so that all the hardware is connected. I also made sure that they worked by connecting the Arduino board to my computer and seeing that the lights of the parts that had them were on.

### Challenges
The main problem that I faced was learning how to connect the pins together. Each component needed multiple connections to various other parts. I learned about where they needed to go and why.

### Next Steps
With the wiring finished, I can move on to the software for my project, which will use mathematical calculations and astronomical data to determine where in the celestial sphere are the planets. Then, the servos will move to the 

For your first milestone, describe what your project is and how you plan to build it. You can include:
- An explanation about the different components of your project and how they will all integrate together
- Technical progress you've made so far
- Challenges you're facing and solving in your future milestones
- What your plan is to complete your project

# Schematics 
Here's where you'll put images of your schematics. [Tinkercad](https://www.tinkercad.com/blog/official-guide-to-tinkercad-circuits) and [Fritzing](https://fritzing.org/learning/) are both great resoruces to create professional schematic diagrams, though BSE recommends Tinkercad becuase it can be done easily and for free in the browser. 

# Code
Here's where you'll put your code. The syntax below places it into a block of code. Follow the guide [here]([url](https://www.markdownguide.org/extended-syntax/)) to learn how to customize it to your project needs. 

```c++
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
```

# Bill of Materials
Here's where you'll list the parts in your project. To add more rows, just copy and paste the example rows below.
Don't forget to place the link of where to buy each component inside the quotation marks in the corresponding row after href =. Follow the guide [here]([url](https://www.markdownguide.org/extended-syntax/)) to learn how to customize this to your project needs. 

| **Part** | **Note** | **Price** | **Link** |
|:--:|:--:|:--:|:--:|
| Elegoo MEGA R3 Board | Contains code, gathers information, calls actions | $22.99 | <a href="https://www.amazon.com/ELEGOO-ATmega2560-ATMEGA16U2-Arduino-Compliant/dp/B01H4ZDYCE/ref=sr_1_2_sspa?crid=1T53YUJ9OSSYN&dib=eyJ2IjoiMSJ9.r70JjFX8-805Ic29MiqGyIVrIwfR90S-MU_43sZp-KAAVlcNAEOr1T7rwwAVzsvcHYga9dGGK0BXwihyPfPnVkL-oHYhHvR4EqifUFiWe18iVrBPIxtgxLOfTWVFeFkJ_wYOAIF6morxGyUOCkb7Mv03nJ13hyW4g7lgQ0ED57YvOHz6QQxS7sDrnHCocjaue2R8aYYmtOcClzcG0fsbSfkAFyo61HtpQTykT-43P6o.34VkovS5jdwrKuRlvnvXA0BTvsBoZuyzxBxjrIDFRiM&dib_tag=se&keywords=elegoo%2Bcontroller%2Bboard&qid=1750204394&sprefix=elegoo%2Bcontroller%2Bboa%2Caps%2C138&sr=8-2-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&th=1"> Link </a> |
| 2 Micro Servos | Moves to point to a specific planet | $6.99 \(for 2) | <a href="https://www.amazon.com/Sipytoph-Helicopter-Airplane-Walking-Control/dp/B09185SC1W/ref=sr_1_8?crid=EG34UOQG1ZTB&keywords=micro+servo+sg90&qid=1689876515&sprefix=micro+servo+sg%2Caps%2C410&sr=8-8"> Link </a> |
| Laser Diode | Points to planets | $6.79 \(for 10) | <a href="https://www.amazon.com/Arduino-A000066-ARDUINO-UNO-R3/dp/B008GRTSV6/"> Link </a> |
| GT-U7 GPS Module | Provides the location and time | $18.99 | <a href="https://www.amazon.com/Navigation-Positioning-Sensitivity-Consumption-High-Precision/dp/B08MZ2CBP7/ref=sr_1_3?crid=EUNF72IZZWV6&keywords=gt-u7&qid=1689876233&sprefix=gt-u%2Caps%2C606&sr=8-3"> Link </a> |
| Button | Switches between tracking and trajectory mode | $5.99 \(for 120) | <a href="https://www.amazon.com/dp/B07VSNN9S2?_encoding=UTF8&psc=1&ref_=cm_sw_r_cp_ud_dp_MSFKE4VWKZJ5XJ9NN4HA_2"> Link </a> |
| SparkFun 9DoF IMU | Detects compass orientation and adjusts the servos accordingly | $19.95 | <a href="https://www.amazon.com/SparkFun-Breakout-ICM-20948-connection-Accelerometer-Magnetometer/dp/B07VNV3WKL/ref=sr_1_1?crid=1LGYJ3JV9LD70&dib=eyJ2IjoiMSJ9.34764cQ6p-jmLkr3GhwuUft2p42-bwaCQWvtW6GQKp0PtXP2jO3QcJjJjhMM_Wgxb3FDngdMkItrhQREMDplIzq8zrsY7p6J7c_TNlW4_zJIH3diYQoDOrdX146CmsYYGPJw352zWbWPPhyTuUuMXil8arsazfTfrbRdjwiAyXn0g3rrfPkfHIG8hB0u39PpkniX-LKQ6Jbo-USjqMWSBiGwTzGhiWaBGIg6nYvF5Vd9MFDr9UgW0stDiTzraU6z.P1sUR-HRp05hkjn9ld2r5K4xRAj7EfXCN2LaUlHcKQA&dib_tag=se&keywords=sparkfun+imu&qid=1750692938&s=industrial&sprefix=sparkfun+i%2Cindustrial%2C324&sr=1-1"> Link </a> |

# Starter Project

<iframe width="560" height="315" src="https://www.youtube.com/embed/RcaV0-HjO3g?si=7L-r6T0KN0yvXpgU" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

<img src="retroimg.png" width="300" height="400">

### Components

- Some challenges that I faced were the wires and the soldering. The wires had to be cut to a certain length, not too long to dangle everywhere, but not too short as to strain the hit of the console's frame. I made sure to measure and test multiple times before I cut the wires. Additionally, many of the soldering components were very near each other. Therefore, I had to adjust my angle of approach in order to get a good-quality solder without creating a short circuit by connecting two adjacent joints.
- Next, I will begin my intensive project, which will be the real time planetary tracker.

### Schematic
<img src="retroschematic.jpg" width="600" height="500">

<!--
# Other Resources/Examples
One of the best parts about Github is that you can view how other people set up their own work. Here are some past BSE portfolios that are awesome examples. You can view how they set up their portfolio, and you can view their index.md files to understand how they implemented different portfolio components.
- [Example 1](https://trashytuber.github.io/YimingJiaBlueStamp/)
- [Example 2](https://sviatil0.github.io/Sviatoslav_BSE/)
- [Example 3](https://arneshkumar.github.io/arneshbluestamp/)

To watch the BSE tutorial on how to create a portfolio, click here.
-->
