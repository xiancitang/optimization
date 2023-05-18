
Project 4

Requirements Implemented:

[Core Items]
1-  yes : Furnitures include 4 pieces: Coach, TV, Table, Plant
2-  yes : In MCMC.cpp function Cd(*) calculate pairwise distance cost , functon Ca(*) calculate pairwise angle cost  
		  widgets that pass in target values and weights: TV_COACH_TARGET_DISTANCE,TARGET_TV_TO_COACH,TARGET_COACH_TO_TV,WD, WA,TEMPERATURE
3 - yes : Reset button will reset all furnitures positions, all furnitures will be within room; in MCMC.cpp function bordersT(*) and bordersR (*)
		  will adjust furniture position and oritentation when necessary
4-  yes : The main logic of stochasitc optimization is in random_search(*) function in MCMC.cpp

[Other Items]

1-  yes : It is checkbox 'ShowUpdate' located under MCMC menu button; first check this checkbox, then click start button under MCMC menu button ; 
		  intermediate results will be displayed every 50 interations
2-  yes : Please check top red line text in Model window during optimization
3-  yes : after run optimazation, a test.csv will be generated under project directory automatically.

