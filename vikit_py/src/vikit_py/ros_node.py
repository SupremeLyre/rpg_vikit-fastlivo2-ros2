#!/usr/bin/env python3

import subprocess

class RosNode:
    def __init__(self, package, executable):
        self._package = package
        self._executable = executable
        self._parameters = []
    
    def add_parameters(self, namespace, parameter_dictionary):
        for key in parameter_dictionary.keys():
            if type(parameter_dictionary[key]) is dict:
                self.add_parameters(namespace+key+'.', parameter_dictionary[key])
            else:
                self._parameters.extend(['-p', namespace+key+':='+str(parameter_dictionary[key])])
        
    def run(self, parameter_dictionary, namespace=''):
        self._parameters = []
        self.add_parameters(namespace, parameter_dictionary)
        print('Starting ROS2 node with parameters: '+' '.join(self._parameters))
        
        subprocess.run(['ros2', 'run', self._package, self._executable, '--ros-args'] + self._parameters, check=True)
        print('ROS2 node finished processing.')
