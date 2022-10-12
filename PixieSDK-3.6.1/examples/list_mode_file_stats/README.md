# List-Mode File Stats
This program provides an example of how to use the PixieData library to decode a list-mode data file
produced by our example software. The example software produces a dedicated data file for each 
module in the system. That way we never have a data file that has mixed decoding configurations. 
Attempting to decode a data file **not** produced by our example software may produce errors.

## Configuration File
The configuration file for the program is simple. It's a JSON array with a single element. 
```json
[
  {
    "slot": 12,
    "revision": 46908,
    "frequency": 250
  }
]
```
Where slot is the Slot ID of the module represented in the data. The `revision` is the DSP firmware 
revision that collected the data. The `frequency` is the ADC sampling frequency of the module that 
collected the data. 

## Usage
```shell
list_mode_file_parser {OPTIONS}

    Validates list-mode data files produced by the example software.

  OPTIONS:

      arguments
        -c[cfg], --config=[cfg]           The configuration file telling us what
                                          to expect in the list-mode data.
        -h, --help                        Displays this message
        -i[input_file],
        --input-file=[input_file]         The input file that we'll attempt to decode.
```
