## Travel Agency

<hr />

### Index
- [Problem Statement](#problem-statement)
- [My code summary](#my-code-summary)
- Scope for Improvements
- [To Do](#to-do)
- Does it have a licence
- References
- Links/ Resources

### [Problem Statement](#index)

CSD102  January – May 2022

Assignment 2: Tour management

submit by 06/02/2022

Noida Tour company arranges seven day tours to Darjeeling, Leh and Ooty. The tourists can be single or members of a group. Each group will have a group leader. Using linked lists data structure, develop a software in C that provides a facility to organize  lists containing name of individual/group leader, number of tourists in the group, for a specified date and for a specified destination. All the tours are to start and end in the month of May 2022. Each group of tourists would have a group leader who will be making requests for booking or cancellation. Your program should use the code system indicated below to get various details of the tour requests by reading from the datafile tour.txt. It should flag any wrong data encountered while reading. 

The first entry on the datafile is going to be an integer N which specifies maximum number of tourists allowable on any tour. Code C1 will be used to introduce a tourist spot. It will be followed by TS1 for Darjeeling, TS2 for Leh and TS3 for Ooty. This code information will remain valid for subsequent entries, till it is changed by another occurrence of C1 on the datafile. 

(TS = Train Station)

Code C2 will be followed by an integer indicating booking date request.  It is guaranteed that  requests will arrive in increasing order of dates. Thus, for example, if a request has been made for 14th May, subsequent requests will be only for dates after 14th.

Code C3 will be followed by initials of a group leader followed by number of 
tourists in that group. The group will be added at the bottom of the tour 
list. It is guaranteed that on the datafile, there will be no duplicate 
initials requesting for tours, which means a group leader will be making only
a single request for a tour in the whole month.  Registration for a group will
not be accepted, if this causes the total number of tourists on that tour to
exceed N.

Code C3A is to cater for “premium” tourist groups. This will operate in same
manner as C3 except that this specific group will be put in top most position
of the concerned list.   If C3A operation causes the total number of tourists
to exceed N, then take appropriate action in your program by removing the 
groups at the bottom of the list. 

Code C5 indicates that a group wants to cancel a tour. This code will be
followed by initials of the group leader. The group will be removed from
the list.

Code C8 followed by tourist spot code is a command to print the name of the
spot, followed by  lists for each of relevant dates (in increasing order). 
Print total number of tourists on that tour, followed by tourists’ data. 
The sample output below indicates the style in which the data needs to be 
printed.

Code C9 will cause the program to exit.  Sample input and output files are given below.
[ Hint: First develop  a prototype system which caters to a single tour for a specific date and specific destination]

You need to submit a .c file of your code. It should contain at the top
 the name of  the assignment, 
followed by your name 
and entry number and lab group number. 

If you are submitting the complete system indicate “complete system” at the top, otherwise indicate “prototype system”. Then add “I certify that this is my own work and I have not copied any part of it from other sources.” 

Your code should be sufficiently documented and neatly organized. 
Also at the bottom of the code you have to provide a  large commented section to include one sample datafile used by you followed by program output.

sample input:

12    C1 TS2  C2 14 C3 AA 3 C3 BB 4 C3 CC 4      C8 TS2        C2 14 C3A KK 3       
C2 20 C3 DD 2 C3 EE 4        C8 TS2  
C5 DD           C1 TS1 C2 12  C3 JJ 4  C3 MM 2      C2 26 C3 NN 3 C3 FD 2       
C8 TS1       C8 TS2         C9 


sample output:

TS2:  Leh
May 14: 11 tourists: AA  BB   CC 

<p>------------------------------------------</p>

TS2:  Leh
May 14: 10 tourists: KK  AA    BB
May 20: 6   tourists:   DD   EE  

<p>------------------------------------------</p>
 
TS1:  Darjeeling
May 12: 6 tourists: JJ  MM 
May 26: No trip

<p>------------------------------------------</p>

TS2:  Leh
May 14: 10 tourists: KK  AA    BB
May 20: 4 tourists:      EE   

### [My Code Summary](#index)

It has a function for each code

### [To DO](#index)

- Edit the code such that when wrong date is entered like 26 May, even then Main struct gets created
- Check the number returned by the functions
- Write code from starting using hint given
- One more misunderstanding err to solve, the next date for the trip can be less than prev date if dest is changed;