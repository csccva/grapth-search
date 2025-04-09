#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

/*
This code creats a set of dependency paths based on a input file that gives nodes and some connection to them in the form 

```
A1 -> B1
B1 -> C1
F3 -> A1
D3 -> G3
```
*/

/*
MIT License

Copyright (c) 2025 Cristian-Vasile Achim

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// Function to find all paths while tracking visited nodes
void find_paths(const std::string& start, 
               const std::unordered_map<std::string, std::vector<std::string> >& adj_list, 
               std::vector<std::string>& path,
               std::unordered_set<std::string>& visited, 
               std::vector<std::vector<std::string> >& all_paths,
               std::unordered_map<std::string, bool>& act_dep) {
    
    // This will present starting new branch if the node is already in a tree
    act_dep[start]=false; 

    // Check if the node has been visited in the present call
    if(visited.find(start) != visited.end()) {
        path.push_back(start);
        all_paths.push_back(path);
        //path.pop_back(); //if no copies are used below
        return;
    }

    // Add the node to the path and mark as visited
    path.push_back(start);
    visited.insert(start);

    // Start a search for each neighbour of the node
    if (adj_list.find(start) != adj_list.end()) {
        for (const std::string& neighbor : adj_list.at(start)) {
             std::vector<std::string> new_path=path;
             std::unordered_set<std::string> new_visited=visited;
             find_paths(neighbor, adj_list,new_path, new_visited, all_paths,act_dep);
            //find_paths(neighbor, adj_list,path, visited, all_paths,act_dep);
        }
    }

    // Store the path if this node is th last in the present path
    if (adj_list.find(start) == adj_list.end()) {
        all_paths.push_back(path);
    }

    // Backtrack: remove current node from path and visited set. if no copies are used above
    //path.pop_back();
    //visited.erase(start);
}


int main() {
    std::vector<std::string> left_column;  // To store the left part
    std::vector<std::string> right_column; // To store the right part
    std::ifstream file("dependencies.txt");

    std::set<std::string> unique_values_left;
    std::string s1,s2,s3;

    // Read line by line from the file
    while( file >> s1 >> s3 >>s2) { 
        left_column.push_back(s1);
        right_column.push_back(s2);
        unique_values_left.insert(s1);
    }    
    file.close();

    // Collect all the unique value from the left side
    std::vector<std::string> vleft(unique_values_left.begin(), unique_values_left.end());

    //Build Adjancency List
    std::unordered_map<std::string, std::vector<std::string> > adj_list;
    for(int i=0;i<vleft.size();i++){
        std:: string pstring= vleft[i];
        for(int j=0; j<left_column.size();j++){
            if(pstring== left_column[j]){
                adj_list[pstring].push_back(right_column[j]);
            }
        }
    }

    std::unordered_map<std::string, bool>act_dep;
    for (const auto& node : adj_list) {
        act_dep[node.first]=true;
    }
    std::vector<std::vector<std::string> > all_paths;
    for (const auto& node : adj_list) {
        if(act_dep[node.first]){
            std::vector<std::string> path;
            std::unordered_set<std::string> visited;
            find_paths(node.first, adj_list, path, visited, all_paths,act_dep);
        }
    }

    // Print the found paths
    std::cout << "Paths found: " << all_paths.size()<< std::endl;
    std::set<std::string> unique_paths;
    std::vector<std::string> no_loop_paths, contain_loop_paths, is_loop_paths;
    for(const auto & path : all_paths){
        std::stringstream path_str;
        std::unordered_set<std::string> unique_nodes;
        bool is_loop = false; 
        
        // Check if a node in a path is present more than once
        for (const std::string& node : path) {
            if (unique_nodes.find(node) != unique_nodes.end()) {
                is_loop = true;  
            }
            unique_nodes.insert(node);
            path_str << node << " -> ";
        }
        
        // Clasify the paths 
        std::string path_output = path_str.str();
        path_output = path_output.substr(0, path_output.size() - 4);
        if (is_loop) {
            if(path[0]==path[path.size()-1]){
                is_loop_paths.push_back(path_output);
            }else
            {
                contain_loop_paths.push_back(path_output);
            }
        } else {
            no_loop_paths.push_back(path_output);
        }
    }
    
    //Print the paths based on containingg circular dependency or not
    std::cout << "No circular dependency" << std::endl;
    for(auto ps: no_loop_paths){
        std::cout << ps << std::endl;
    }

    std::cout << "Circular dependeny detected:" << std::endl;
    for(auto ps: is_loop_paths){
        std::cout << ps << std::endl;
    }
    for(auto ps: contain_loop_paths){
        std::cout << ps << std::endl;
    }

    return 0;
}
