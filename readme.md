# Programming Challenges
## Problem 3.6: File Fragmentation[^1]
### Problem description
Several identical files are broken into two pieces each. This program reconstructs the files.

The input contains a line with a single integer, this represents the number of test cases - followed by a blank line. The next several lines describe the fragments of the files - again terminated by a blank line. The final case will be terminated by an EOF.

The output is a string of characters describing the structure of the initial files.


### Example 
>**Input**
>1                                              
>                                        
>011                                       
>0111                                            
>01110                                        
>111                                  
>0111                                   
>10111                                                                                                       

>**Output**                            
>01110111                                     
                                                                            

### Problem solution
First, the dimensions of the initial files are calculated. Then The fragments are divided into pair groups. For example, if the initial length of each file is 4: 1's & 3's will be grouped together; 2's and 2's will be grouped together, etc... All possible combinations for these pairs will be computed. The correct answer will be found in all groups.

### Bug Checklist
- [ ] NA

[^1]: Programming Challenges By S.Skienna and M.Revilla, page 73