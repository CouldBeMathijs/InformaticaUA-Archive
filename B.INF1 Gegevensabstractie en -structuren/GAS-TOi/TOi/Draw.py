import ast
from time import sleep

from graphviz import Digraph


def create_tree_graph(node, graph=None, parent=None):
    """Recursively create a tree graph using Graphviz."""
    if graph is None:
        graph = Digraph(comment='Tree', engine='dot')

    # Create a node for the current root value
    node_id = str(id(node))
    graph.node(node_id, label=" ".join(map(str, node['root'])))

    if parent:
        # If parent exists, create an edge from the parent to this node
        graph.edge(parent, node_id)

    # Recursively add children
    if 'children' in node:
        for child in node['children']:
            create_tree_graph(child, graph, node_id)

    return graph


def visualize_tree_from_input():
    """Accept a string input from the console, parse it, and visualize the tree."""
    # Get the input string from the user
    input_str = input("Enter the tree structure as a Python dictionary: ")

    try:
        # Parse the string to a Python dictionary
        tree_structure = ast.literal_eval(input_str)

        # Validate the structure
        if not isinstance(tree_structure, dict) or 'root' not in tree_structure:
            print("Invalid tree structure. It should be a dictionary with a 'root' key.")
            return

        # Create the Graphviz tree
        graph = create_tree_graph(tree_structure)

        # Render and display the tree to a file
        graph.render('tree_visualization', format='png', view=True)
        print("Tree visualization generated and opened as 'tree_visualization.png'.")

    except (SyntaxError, ValueError) as e:
        print(f"Error parsing input: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")


def visualize_tree(tree_structure, output_file='tree_visualization'):
    """Visualize a tree dictionary as a Graphviz tree."""

    def create_tree_graph(node, graph=None, parent=None):
        """Recursively add nodes and edges to the graph."""
        if graph is None:
            graph = Digraph(comment='Tree', engine='dot')
        node_id = str(id(node))
        graph.node(node_id, label=" ".join(map(str, node['root'])))
        if parent:
            graph.edge(parent, node_id)
        if 'children' in node:
            for child in node['children']:
                create_tree_graph(child, graph, node_id)
        return graph

    if not isinstance(tree_structure, dict) or 'root' not in tree_structure:
        raise ValueError("Invalid tree structure. Must be a dictionary with a 'root' key.")

    graph = create_tree_graph(tree_structure)
    file_path = graph.render(output_file, format='png', cleanup=True)
    print(f"Tree visualization saved to {file_path}.")
    return file_path


def print_tree(obj):
    """Print function replacement for visualizing tree dictionaries."""
    if isinstance(obj, dict) and 'root' in obj:
        try:
            visualize_tree(obj)
            sleep(0)
        except Exception as e:
            print(f"Error visualizing tree: {e}")
    else:
        print(obj)

# Call the function to visualize the tree based on input
visualize_tree_from_input()
