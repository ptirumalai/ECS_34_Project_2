# ECS_34_Project_2

Project by Achintya Jaimini and Pranav Tirumalai

Classes we implemented:

DSVReader: A class for reading delimiter-separated-value (DSV) files. It parses input data based on a specified delimiter and extracts rows into a vector of strings. Handles proper escaping of special characters.

DSVWriter: A class for writing delimiter-separated-value (DSV) files. It formats and writes data to an output stream while ensuring correct handling of delimiters, quotes, and newlines.

XMLReader: A class for reading and parsing XML files using the Expat library. It extracts XML entities, their attributes, and maintains proper parsing structure. Supports skipping character data (CDATA) if required.

XMLWriter: A class for writing well-formed XML files. It generates XML entities, manages nesting of elements, and ensures proper formatting of attributes and content.