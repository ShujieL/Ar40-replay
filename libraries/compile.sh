#!/bin/csh 
set dirs=`ls`
foreach d ($dirs)  
   if(-d $d) then
      cd $d
      make realclean;#make
      cd ..
   endif
end
