# General Description

This program is designed to find the optimal path an elevator can take as to reduce the floors that its users have to climb presuming that the elevator can make limmited stops. The program is able to run the calculations using four fundemental programming methods:  
1) Recursion
2) Brute Force
3) Memoization
4) Dynamic Programming

# Input file format
The program accepts the name of an input file as it's first argument. The file should be formated like so: 

>&lt;people&gt; &lt;stops&gt;  
&lt;stop1&gt; &lt;stop2&gt; &lt;stop3&gt; ....

Where people are the number of people that get inside the elevator, stops is the limmit the elevator has on how many stops it can do and stop1, stop2, stop3 etc is the floor each passenger wants to stop at. The order in which the floors are typed into the file does not matter.

For example, this is a valid input file: 

|------ people  
|&nbsp;&nbsp;|---- stops   
v v   
5 2  
11 2 7 13 7  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;^  
Destinations
# Available Commands

<details>
<summary><span style="font-size: 20px;"><b>recurse</b></span></summary>
This command uses plain recursion without the use of memoization to do it's calculations.

> Usage:&nbsp;&nbsp;&nbsp;./elevate <input.txt> --mode=recurse

The programm relies on the use of the formula:  
 > &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Minimum Cost = min{ M[stops][j] }  
 
 where j runs from 0 to the highest floort he elevator will have to reach  
   
M is calculated with the following formula:  
 >  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;M[i][j] = min{ M[i-1][k] + walking }  
 
 where k runs fom 0 to j and walking consists of three variables: 
 1) The sum of the floors each passenger will have to walk from the k-th floor up to the j-th floor
 2) The sum of the floors each passenger with a destination higher than the j-th floor will have to walk 
 3) The negative sum of the floors each passenger with a destination higher that the k-th floor will have to walk  

 In the case that the elevator does not move at all, i = 0, which means that all the "passengers" will have to take the stairs to their floors, and the value of M is equal to that sum. 

 This layout is perfect for a recursive aproach, seeing as to how each row of costs relies on the values of the previous row of cost and this reaches an endpoint when it hits the case of the elevator not moving at all.

</details>

<details>
<summary><span style="font-size: 20px;"><b>brute</b></span></summary>
This command uses a brute force aproach to do it's calculations.

> Usage:&nbsp;&nbsp;&nbsp;./elevate <input.txt> --mode=brute


This command itterates through every possible unique floor combination and calculated the floors all the passengers will have to walk given those combinations. 

It does this by once again using a recursive process, although this time it's not used for the calculations, but for the selections.  

It starts of by the inputed maximum floor, then goes and selects the first available next stop up until it selects maxstops floors. After that it unselects the last picked floor and picks the next avaialble floor. After it finishes scanning the possibilities for the mastops floor, it unpicks the maxstops-1 selected floor, picks the next available one, and so on.

This will become much clearer with a visual example. 

Lets suppose we have 3 passengers that want to fo to the floors 3 - 5 - 4 respectivly. The elevator can make 2 stops.  

Since the maximum floor the elevator could go is the 5th one, the elevator has 5 possible floors it can go to, and the selections can be visualised as:

> [ &nbsp;][ 1 ][ &nbsp;][ 2 ][ &nbsp;]

First of all it starts by calculating the cost of the elevator not doing any stops, stores it as the current minimum cost and goes to select the first stop:

> [ 1 ][ &nbsp;][ &nbsp;][ &nbsp;][ &nbsp;]

It calculates the cost for this instance and then goes for the second stop it can make

> [ 1 ][ 2 ][ &nbsp;][ &nbsp;][ &nbsp;]

After calculating this, it can not make a 3rd stop so it unpicks it's second choice and moves onto the next choice it can make:

> [ 1 ][ &nbsp;][ 2 ][ &nbsp;][ &nbsp;]

This process continues like so:
> [ 1 ][ &nbsp;][ &nbsp;][ 2 ][ &nbsp;]  
> [ 1 ][ &nbsp;][ &nbsp;][ &nbsp;][ 2 ]  

After it can make no other choices for the 2nd stop, it unpicks both the choices it has made thus far and makes the next possible choice for the first floor: 

> [ &nbsp;][ 1 ][ &nbsp;][ &nbsp;][ &nbsp;]

and continues like so:
> [ &nbsp;][ 1 ][ 2 ][ &nbsp;][ &nbsp;]  
> [ &nbsp;][ 1 ][ &nbsp;][ 2 ][ &nbsp;]  
> [ &nbsp;][ 1 ][ &nbsp;][ &nbsp;][ 2 ]  
> [ &nbsp;][ &nbsp;][ 1 ][ &nbsp; ][ &nbsp; ]  
> [ &nbsp;][ &nbsp;][ 1 ][ 2 ][ &nbsp;]  
> [ &nbsp;][ &nbsp;][ 1 ][ &nbsp;][ 2 ]  
> [ &nbsp;][ &nbsp;][ &nbsp;][ 1 ][ &nbsp;&nbsp; ]  
> [ &nbsp;][ &nbsp;][ &nbsp;][ 1 ][ 2 ]  
> [ &nbsp;][ &nbsp; ][ &nbsp;][ &nbsp; ][ 1 ]

At the end it has calculated the minimum cost. Also since this option iterates through every possible combination of stops, it can also store and output the exact path the elevator takes.

</details>


<details>
<summary><span style="font-size: 20px;"><b>memoize</b></span></summary>
This command uses recursion with the use of memoization to do the calculations.

> Usage:&nbsp;&nbsp;&nbsp;./elevate <input.txt> --mode=memoize

This method follows the exact same logic as recursion, with the addition of storing and accessing the values it has already calculated, thus avoiding uneccasery re-calculations.  

The difference between the two aproaches is especialy evident in the ammount of time each takes to calculate a hard combination. For example recurse can sort out the following input file (input8.txt) in a matter of 1-6 minutes, depending on the computer it runs, where the recusrive aproach can do it in less than a second! (Takes 4 minutes on the linux labs computers)

>25 7  
1 2 2 3 5 6 6 8 10 13 15 15 16 17 18 18 18 20 22 25 30 38 49 55 62  


This is the result of the branching that happens each time a value is calculated recursively. Elaborating on this further, remembering the formula that was described in the briefing of recurse, when calculating M[i][j] we need to find the minimum value between M[i-1][0] , M[i-1][1], M[i-1][2], M[i-1][3], ... , M[i-1][j], where each of those is also calculated using recursion. This creates a lot of unecassery calculations since each of the M[i-1][x] need the values of M[i-2][x].

Avoiding these calculations by using memoization. naturally, drastically reduces the time the program takes to do the calculations.


</details>

<details>
<summary><span style="font-size: 20px;"><b>dp</b></span></summary>
This command utilizes Dynamic Programming to find the optimal stops.

> Usage:&nbsp;&nbsp;&nbsp;./elevate <input.txt> --mode=dp

While the memoization approach works top-down by breaking the problem into sub-problems as it encounters them, the Dynamic Programming approach works bottom-up. It systematically fills a table M starting from the simplest case (0 stops) and builds up to the maximum number of allowed stops.

The program constructs a 2D table where each entry M[i][j] stores two pieces of information:
1)  **The Value:** The minimum cost to reach floor $j$ using exactly $i$ stops.
2)  **The Parent Floor:** The floor the elevator was at during the (i-1)-th stop to achieve that minimum cost.

This approach is highly efficient because it both avoids using the stack frame and calculates each value only after all its components are calculated

**Backtracking the Path:**  
One of the key advantages of this method is the ability to reconstruct the exact floors the elevator stopped at. Once the table is fully populated, the program finds the minimum cost in the final row (the row representing the maximum stops). From there, it follows the "Parent Floor" pointers back up through the rows to identify the sequence of stops.

**Visualizing the Backtracking:**
We cam imagine the table as a grid where each cell points to a cell in the row above it:

> **Row 2 (Stop 2):** [Floor 7, Parent: 3] <- Minimum total cost found here
>
> **Row 1 (Stop 1):** [Floor 3, Parent: 0] <- Followed pointer from Row 2
>
> **Row 0 (Stop 0):** [Floor 0] <- Start point

By following these pointers from the bottom up, the program identifies that the optimal path was **0 -> 3 -> 7**.

</details>