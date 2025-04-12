from collections import defaultdict

def find_paths(start, adj_list, path=None, visited=None):
    if path is None:
        path = []
    if visited is None:
        visited = set()

    path = path + [start]
    visited = visited | {start}

    paths = []

    if start in adj_list:
        for neighbor in adj_list[start]:
            if neighbor in visited:
                paths.append(path + [neighbor])  # Handle loops
            else:
                new_paths = find_paths(neighbor, adj_list, path, visited)
                paths.extend(new_paths)

    if not paths:
        return [path]
    
    return paths


def main():
    left_column = []
    right_column = []
    unique_values_left = set()

    with open("dependencies.txt", "r") as f:
        for line in f:
            tokens = line.strip().split()
            if len(tokens) == 3:
                s1, _, s2 = tokens
                left_column.append(s1)
                right_column.append(s2)
                unique_values_left.add(s1)

    # Build adjacency list
    adj_list = defaultdict(list)
    for i in range(len(left_column)):
        adj_list[left_column[i]].append(right_column[i])

    # Keep track of whether a node is an active dependency root
    act_dep = {node: True for node in adj_list}

    all_paths = []

    for node in adj_list:
        if act_dep[node]:
            paths_from_node = find_paths(node, adj_list)
            all_paths.extend(paths_from_node)
            # Mark all nodes in these paths as visited so we don’t restart from them
            for path in paths_from_node:
                for pnode in path:
                    act_dep[pnode] = False

    print(f"Paths found: {len(all_paths)}")

    # Classify and print paths
    no_loop_paths = []
    is_loop_paths = []
    contain_loop_paths = []

    for path in all_paths:
        unique_nodes = set()
        is_loop = False

        for node in path:
            if node in unique_nodes:
                is_loop = True
            unique_nodes.add(node)

        path_str = " -> ".join(path)
        if is_loop:
            if path[0] == path[-1] and len(path) > 1:
                is_loop_paths.append(f"{path_str} (Pure loop)")
            else:
                contain_loop_paths.append(f"{path_str} (Contains a loop)")
        else:
            no_loop_paths.append(f"{path_str} (No loop detected)")

    print("No circular dependency:")
    for ps in no_loop_paths:
        print(ps)

    print("Circular dependency detected:")
    for ps in is_loop_paths:
        print(ps)
    for ps in contain_loop_paths:
        print(ps)


if __name__ == "__main__":
    main()
