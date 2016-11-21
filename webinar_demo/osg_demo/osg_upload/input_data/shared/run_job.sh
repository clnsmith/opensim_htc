#!/bin/sh
#Run Passive Simulation

#Extract Files
#tar -xzf shared_inputs.tar.gz
#tar -xzf job_inputs.tar.gz

#Save input file names
in_files=(*)

#Load OpenSim
module load opensim

#Forward Simulation
forward -L ./UWLigamentPlugin -S forward_settings.xml

#Remove Input Files
rm "${in_files[@]}"
rm -rf tmp/

#Compress Results Files
out_files=(*)
tar -czf job_results.tar.gz *
rm "${out_files[@]}"
