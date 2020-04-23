[![Build Status](https://travis-ci.org/gyselroth/docxbox.svg?branch=master)](https://travis-ci.org/gyselroth/docxbox)

docxBox
=======

Linux tool for DOCX (OpenXML) analysis and manipulation.


## Table of contents

* [Planned features](#planned-features)
* [Commands](#commands)
  * [Output DOCX contents](#output-docx-contents)
    + [List files](#list-files)
    + [List meta data](#list-meta-data)
    + [Reference: Recognized meta attributes](#reference-recognized-meta-attributes)
    + [List referenced fonts](#List-referenced-fonts)
    + [List images information](#list-images-information)
    + [List fields](#list-fields)
  * [Output plaintext](#output-plaintext)
  * [Modify document](#modify-document)
      + [Modify meta data](#modify-meta-data)
      + [Replace image](#replace-image)
      + [Replace text by text](#replace-text-by-text)
      + [Replace text by image](#replace-text-by-image)
      + [Replace text by table](#replace-text-by-table)
      + [Remove content between text](#remove-content-between-text)
      + [Randomize document text](#randomize-document-text)
  * [Unzip DOCX: All files, or only media files, format XML](#unzip-docx-all-files-or-only-media-files-format-xml)
  * [Zip files into DOCX](#zip-files-into-docx)  
  * [Output docxBox help or version number](#output-docxbox-help-or-version-number)  
* [Build Instructions](#build-instructions)
* [Running Tests](#running-tests)
* [Code Convention](#code-convention)
* [Changelog](#changelog)
* [Bug Reporting and Feature Requests](#bug-reporting-and-feature-requests)
* [Third Party References](#third-party-references)
* [License](#license)


Planned Features
----------------

* v0.0.3: Replace text by image
* v0.0.3: Replace field by text, table and image
* v0.0.4: Batch process sequences of manipulation operations
* v0.0.5: Ensure microsoft word compatibility
* v0.1.0: Add optional configuration options via environment vars
* v0.1.0: (Optional) logging of operations
* v1.0.0: Libre-Office compatible appending of two DOCX files into a single one 
  (by XML appending, instead of adding sub-documents) 


Commands
--------

### Output DOCX contents

#### List files

Lists files (and directories) contained within a given DOCX, and their attributes:

````docxbox ls foo.docx````

To output as JSON:

````docxbox lsj foo.docx````  
or ````docxbox ls foo.docx -j````  
or ````docxbox ls foo.docx --json````  

#### List meta data

docxBox displays only attributes that are contained within the current 
DOCX file (this differs by DOCX version and application), also if given empty.

Output meta data of given DOCX:  

````docxbox lsm foo.docx````  
or ````docxbox ls foo.docx -m````  
or ````docxbox ls foo.docx --meta````

To output as JSON:

````docxbox lsmj foo.docx````  
or ````docxbox lsm foo.docx -j````  
or ````docxbox ls foo.docx -mj````  
or ````docxbox lsm foo.docx --json````  
or ````docxbox ls foo.docx --meta --json````    

  
#### Reference: Recognized meta attributes

* **Authors:** Creator, lastModifiedBy (``<dc:creator>`` and ``<cp:lastModifiedBy>`` of core.xml)
* **Dates** (ISO 8601): Creation-, modification and print-date  
  (``<dcterms:created>`` and ``<cp:modified>`` and ``<cp:lastPrinted>`` of core.xml) 
* **Descriptions:** Description, Keywords, Subject, Title   
  (``<dc:description>``, ``<dc:keywords>``, ``<dc:subject>``, ``<dc:title>`` of core.xml)
* **Language** (``<dc:language>`` of core.xml) 
* **Revision** (``<cp:revision>`` of core.xml)
* **Application** created with and its version, name of used template, company,
  XML schema of document (``<Application>``, ``<AppVersion>``, ``<Template>``, 
  ``<Properties xmlns ...`` and ``<Company>`` of app.xml)


#### List referenced fonts

````docxbox lsf foo.docx````  
or ````docxbox ls foo.docx -f````  
or ````docxbox ls foo.docx --fonts````  

To output as JSON:

````docxbox lsfj foo.docx````  
or ````docxbox lsf foo.docx -j````
or ````docxbox ls foo.docx -fj````  
or ````docxbox lsf foo.docx --json````  
or ````docxbox ls foo.docx --fonts --json````


#### List images information

Output list of contained images

````docxbox lsi foo.docx````  
or ````docxbox ls foo.docx -i````  
or ````docxbox ls foo.docx --images````  

To output as JSON:

````docxbox lsij foo.docx````  
or ````docxbox lsi foo.docx -j````  
or ````docxbox ls foo.docx -ij````  
or ````docxbox lsi foo.docx --json````  
or ````docxbox ls foo.docx --images --json````  

#### List fields

````docxbox lsd foo.docx````  
or ````docxbox ls foo.docx -d````  
or ````docxbox ls foo.docx --fields````  

To output as JSON:  

````docxbox lsdj foo.docx````  
or ````docxbox ls foo.docx -dj````  
or ````docxbox lsd foo.docx --json````  
or ````docxbox ls foo.docx --fields --json````  


### Output plaintext

````docxbox txt foo.docx```` outputs the text from document (ATM: w/o header and footer)

**Output plaintext segments:**  
````docxbox txt foo.docx -s````   
or ````docxbox txt foo.docx --segments````   

Outputs the text from document, w/ markup sections separated by newlines.
This can be helpful to identify "segmented" sentences:
Sentences which visually appear as a unit, but are segmented into separate XML 
parent elements for formatting.


### Modify document

#### Modify meta data

DocxBox allows to modify existing attributes, or adds attributes if not present.

* Set creation-date:            ````docxbox mm foo.docx created "2020-01-29T09:21:00Z"````
* Set creator attribute:        ````docxbox mm foo.docx creator "docxBox v0.0.1"````  
* Set description attribute:    ````docxbox mm foo.docx description "Foo bar baz"````  
* Set keywords attribute:       ````docxbox mm foo.docx keywords "Foo bar baz"````  
* Set language attribute:       ````docxbox mm foo.docx language "en-US"````  
* Set lastModifiedBy attribute: ````docxbox mm foo.docx lastModifiedBy "docxBox v0.0.1"````
* Set lastPrinted attribute:    ````docxbox mm foo.docx lastPrinted "2020-01-10T10:31:00Z"````
* Set modification-date:        ````docxbox mm foo.docx modified "2020-01-29T09:21:00Z"````
* Set revision attribute:       ````docxbox mm foo.docx revision 2````
* Set subject attribute:        ````docxbox mm foo.docx subject "Foo bar"````
* Set title attribute:          ````docxbox mm foo.docx title "Foo bar, baz"````

To alter/insert an attribute and save the modified document to a new file:  
````docxbox mm foo.docx <attribute> <value> new.docx````


#### Replace image

````docxbox rpi foo.docx image1.jpeg /home/replacement.jpeg````  
This overwrites the original DOCX with the modified document.

**Note:** For now the original and replacement image must be of the same format 
(bmp, gif, jpg, etc.).

````docxbox rpi foo.docx image1.jpeg /home/replacement.jpeg new.docx````  
This creates a new file: new.docx


#### Replace text by text

Replace all (case-sensitive) occurrences of given string in DOCX text:

````docxbox rpt foo.docx old new```` updates foo.docx  
````docxbox rpt foo.docx old new new.docx```` creates a new file new.docx  


#### Replace text by image

**Example:** To replace text by an image: 

The image specification as JSON looks like:

````
{
    "img":{
        "filename":"example.jpg",
        "offset":[0,0],
        "size":[2438400,1828800]
    }
}
````

##### Specification rules:
* JSON must be wrapped within ``{...}``
* The type specifier (``img``) is mandatory
* When adding a new image file resource, the ``filename`` parameter is optional,
  it specifies a different filename to be used within DOCX 
* The ``offset`` argument is optional
* Embeddable image formats are: ``bmp``, ``emg``, ``gif``, ``jpeg``, ``jpg``, ``png``, ``tif``, ``tiff``, ``wmf``
* Size is given in EMUs (English Metric Unit) that is: ``pixels * 9525``

The image configuration must be passed as escaped JSON, 
when inserting a new image file, it must be given as additional argument:  
````docxbox rpt foo.docx search "{\"image\":{\"size\":[2438400,1828800]}}" images/ex1.jpg````


#### Replace text by table

**Example:** To replace text by a newly rendered table like:

| A  | B  | C  |
|----|----|----|
| a1 | b1 | c1 |
| a2 | b2 | c2 |
| a3 | b3 | c3 | 

the table specification as JSON looks like:
````
{
    "table":{
        "columns":3,
        "rows":3,
        "header":["A","B","C"],
        "content":[
            ["a1","b1","c1"],
            ["a2","b2","c2"],
            ["a3","b3","c3"]
        ]
    }
}
````

##### Specification rules:

* JSON must be wrapped within ``{...}``
* The type specifier (``table``) is mandatory
* The order of keys within the config of the type (``table``) is arbitrary
* ``header`` is optional, when given: ``columns`` is optional
* ``content`` is optional, when given: ``rows`` is optional

The table configuration must be passed as escaped JSON:    
````docxbox rpt foo.docx search "{\"table\":{\"header\":[\"A\",\"B\",\"C\"],\"content\":[[\"a1\",\"a2\",\"a3\"],[\"b1\",\"b2\",\"b3\"],[\"c1\",\"c2\",\"c3\"]]}}"````
  

#### Remove content between text

Remove content between (and including) given strings (``left`` and ``right``):    

````docxbox rem foo.docx left right```` updates foo.docx  
````docxbox rem foo.docx left right new.docx```` creates a new file new.docx


#### Randomize document text

Replace all text of an existing document by similarly structured random 
"Lorem Ipsum" dummy text, helpful for generating DOCX documents for testing 
purposes:

````docxbox lorem foo.docx```` updates foo.docx  
````docxbox lorem foo.docx new.docx```` creates a new file new.docx  


### Unzip DOCX: All files, or only media files, format XML

**Unzip all files:** ````docxbox uz foo.docx````  

**Unzip only media files:**  
````docxbox uzm foo.docx````
or ````docxbox uz foo.docx -m````  
or ````docxbox uz foo.docx --media````    

**Unzip all files and indent XML files:**  
````docxbox uzi foo.docx````  
or ````docxbox uz foo.docx -i````  
or ````docxbox uz foo.docx --indent````  


### Zip files into DOCX

````docxbox zp path/to/directory out.docx````


### Output docxBox help or version number

````docxbox````  
or ````docxbox h````    
Outputs docxBox's help text.

````docxbox h <command>```` Outputs more help on a given command.  

````docxbox v```` Outputs the installed docxBox's version number.


Build Instructions
------------------

`cmake CMakeLists.txt; make`


Running tests
-------------

In order to run functional tests, [Bats](https://github.com/sstephenson/bats) must be installed.

Run all tests: `./test.sh`


Code Convention
---------------

The source code of docxBox follows the Google C++ Style Guide, see: https://google.github.io/styleguide/cppguide.html  
The source code of functional tests mostly (excluding rule for maximum characters per line) follows the Google Shell Style Guide, see https://google.github.io/styleguide/shell.xml


Changelog
---------

See [Changelog](https://github.com/gyselroth/docxbox/blob/master/CHANGELOG.md)


Bug Reporting and Feature Requests
----------------------------------

If you find a bug or have an enhancement request, please file an issue on the 
github repository.


Third Party References
----------------------

Microsoft Office and Word are registered trademarks of Microsoft Corporation.


docxBox was built using the following third party libraries and tools:

| Library                                                         | Description                                                                | License                                                          |
| --------------------------------------------------------------- | -------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| [nlohmann/json](https://github.com/nlohmann/json)               | JSON for Modern C++                                                        | [MIT License](https://opensource.org/licenses/MIT)               |
| [tfussel/miniz-cpp](https://github.com/tfussell/miniz-cpp)      | Cross-platform header-only C++14 library for reading and writing ZIP files | [MIT License](https://opensource.org/licenses/MIT)               |
| [leethomason/tinyxml2](https://github.com/leethomason/tinyxml2) | A simple, small, efficient, C++ XML parser                                 | [zlib License](https://www.zlib.net/zlib_license.html)           |

| Tool                                                            | Description                                                                | License                                                                                                   |
| --------------------------------------------------------------- | -------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| [Bats](https://github.com/sstephenson/bats)                     | Bash Automated Testing System                                              | [MIT License](https://opensource.org/licenses/MIT)                                                        |      
| [Clang](https://clang.llvm.org/)                                | A C language family frontend for LLVM                                      | [Apache License](https://www.apache.org/licenses/)                                                        |      
| [Cmake](https://cmake.org/)                                     | Family of tools designed to build, test and package software               | [New BSD License](https://en.wikipedia.org/wiki/New_BSD_License)                                          |      
| [GCC](https://gcc.gnu.org/)                                     | GCC, the GNU Compiler Collection                                           | [GNU General Public License version 3](https://gcc.gnu.org/onlinedocs/libstdc++/manual/appendix_gpl.html) |      
| [Travis CI](https://travis-ci.org)                              | Hosted Continuous Integration Service                                      | [MIT License](https://opensource.org/licenses/MIT)                                                        |

Thanks a lot!


License
-------

docxBox is licensed under [The MIT License (MIT)](https://opensource.org/licenses/MIT)
