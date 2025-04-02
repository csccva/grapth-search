#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

//using namespace std;

    


// Function to find all paths while tracking visited nodes
void find_paths(const std::string& start, 
               const std::unordered_map<std::string, std::vector<std::string>>& adj_list, 
               std::vector<std::string>& path,
               std::unordered_set<std::string>& visited, 
               std::vector<std::vector<std::string>>& all_paths,
               std::unordered_map<std::string, bool>& act_dep) {
    
    act_dep[start]=false; // avoid starting paths if the nodes are already part of other loops
    // Loop detection
    if(visited.find(start) != visited.end()) {
        path.push_back(start);
        all_paths.push_back(path);
        return;
    }

    // Extend the path and mark the node as visited
    path.push_back(start);
    visited.insert(start);

    // Explore neighbors
    if (adj_list.find(start) != adj_list.end()) {
        for (const std::string& neighbor : adj_list.at(start)) {
            std::vector<std::string> new_path=path;
            std::unordered_set<std::string> new_visited=visited;
            find_paths(neighbor, adj_list,new_path, new_visited, all_paths,act_dep);
        }
    }

    // If no further paths are found (i.e., it's a leaf node), store the current path
    if (adj_list.find(start) == adj_list.end()) {
        all_paths.push_back(path);
    }

    // Backtrack: remove current node from path and visited set
    path.pop_back();
    visited.erase(start);
}


int main() {
    std::vector<std::string> leftColumn;  // To store the left part
    std::vector<std::string> rightColumn; // To store the right part
    std::ifstream file("dependencies.txt");
    std::string line, left, right;
    std::string delimiter = "->";  // The delimiter
    std::set<std::string> uniqueValues,uniqueValuesleft,uniqueValuesright;
    std::string s1,s2,s3;

    while( file >> s1 >> s3 >>s2) { // read line by line from the file
        leftColumn.push_back(s1);
        rightColumn.push_back(s2);
        uniqueValues.insert(s1);
        uniqueValues.insert(s2);
        uniqueValuesleft.insert(s1);
        uniqueValuesright.insert(s2);
    }    
    file.close();

/*     for (int i = 0; i < leftColumn.size(); i++) {
        std::cout << leftColumn[i] << "    " << rightColumn[i] << std::endl;  
    } */

    std::vector<std::string> v(uniqueValues.begin(), uniqueValues.end());
    // std::cout << "\nUnique Values:\n";
    // for (const std::string& val : v) {
    //     std::cout << val << std::endl;
    // }

    std::vector<std::string> vleft(uniqueValuesleft.begin(), uniqueValuesleft.end());
/*     std::cout << "\nUnique Values Left:\n";
    for (const std::string& val : vleft) {
        std::cout << val << std::endl;
    } */

    std::vector<std::string> vright(uniqueValuesright.begin(), uniqueValuesright.end());
/*     std::cout << "\nUnique Values Right:\n";
    for (const std::string& val : vright) {
        std::cout << val << std::endl;
    } */

    //Adjancency list
    std::unordered_map<std::string, std::vector<std::string>> adj_list;
    std::vector<std::vector<std::string>> adj(vleft.size()); // only use the unique left index
    for(int i=0;i<vleft.size();i++){
        std:: string pstring= vleft[i];
        for(int j=0; j<leftColumn.size();j++){
            if(pstring== leftColumn[j]){
                adj[i].push_back(rightColumn[j]);
                adj_list[pstring].push_back(rightColumn[j]);
            }
        }
    }

    std::cout << "Adjacency list for the Graph: " << std::endl;
    for(int i=0;i<adj.size();i++){
        std::cout << vleft[i] << " --> ";
        for(auto j : adj[i]){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

/*     std::cout << "Input data: " << std::endl;
    for(int i=0;i<leftColumn.size();i++){
        std::cout << leftColumn[i] << " --> " << rightColumn[i] << std::endl ;
    }
     */
    std::unordered_map<std::string, bool>act_dep;
    for (const auto& node : adj_list) {
        act_dep[node.first]=true;
    }
    std::vector<std::vector<std::string>> all_paths;
    for (const auto& node : adj_list) {
        if(act_dep[node.first]){
            std::vector<std::string> path;
            std::unordered_set<std::string> visited;
            find_paths(node.first, adj_list, path, visited, all_paths,act_dep);
        }
    }

    std::cout << "Paths found: " << all_paths.size()<< std::endl;
    std::set<std::string> unique_paths;
    std::vector<std::string> no_loop_paths, contain_loop_paths, is_loop_paths;
    for(const auto & path : all_paths){
        std::stringstream path_str;
        std::unordered_set<std::string> unique_nodes;
        bool is_loop = false; 

        for (const std::string& node : path) {
            if (unique_nodes.find(node) != unique_nodes.end()) {
                is_loop = true;  // Contains a loop
            }
            unique_nodes.insert(node);
            path_str << node << " -> ";
        }
        std::string path_output = path_str.str();
        path_output = path_output.substr(0, path_output.size() - 4);
        // Check if the path is already printed
        //if (unique_paths.find(path_output) == unique_paths.end()) 
        {
            //unique_paths.insert(path_output);
            if (is_loop) {
                if(path[0]==path[path.size()-1]){
                    is_loop_paths.push_back(path_output);
                    //std::cout << "(Is a loop): " << path_output << std::endl;
                }else
                {
                    contain_loop_paths.push_back(path_output);
                    //std::cout<< "(Contains a loop): "  << path_output << std::endl;
                }
            } else {
                no_loop_paths.push_back(path_output);
                //std::cout << "(No loop detected): " << path_output << std::endl;
            }
        }

    }
    
    std::cout << "No circular dependency" << std::endl;
    for(auto p: no_loop_paths){
        std::cout << p << std::endl;
    }

    std::cout << "Circular dependeny detected:" << std::endl;
    for(auto p: is_loop_paths){
        std::cout << p << std::endl;
    }
    for(auto p: contain_loop_paths){
        std::cout << p << std::endl;
    }

    return 0;
}
