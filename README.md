[![Build Status](https://travis-ci.org/gyselroth/docxbox.svg?branch=master)](https://travis-ci.org/gyselroth/docxbox)

docxBox
=======

Linux tool for DOCX (Office Open XML) analysis, manipulation and templating.


## Table of contents

* [Commands](#commands)
  * [Unzip DOCX: All files, or only media files, format XML](#unzip-docx-all-files-or-only-media-files-format-xml)
  * [Zip files into DOCX](#zip-files-into-docx)
  * [Output DOCX contents](#output-docx-contents)
    + [List files: All, filtered, images only](#list-files-all-filtered-images-only)
    + [List meta data](#list-meta-data)
    + [List referenced fonts](#List-referenced-fonts)
    + [List fields](#list-fields)
    + [Output XML](#output-xml)
    + [Output document as plaintext](#output-document-as-plaintext)
  * [Compare DOCX documents](#compare-docx-documents)
  * [Modify document](#modify-document)
      + [Modify meta data](#modify-meta-data)
      + [Replace image](#replace-image)
      + [Replace text](#replace-text)
      + [Replace by markup](#replace-by-markup)
        + [Insert heading](#insert-heading)
        + [Insert text](#insert-text)
        + [Insert paragraph containing text](#insert-paragraph-containing-text)
        + [Insert hyperlink](#insert-hyperlink) 
        + [Insert image](#insert-image)
        + [Insert list](#insert-list)
        + [Insert table](#insert-table)
      + [Remove content between text](#remove-content-between-text)
      + [Set field value: Merge fields, generic fields](#set-field-value-merge-fields-generic-fields)
      + [Randomize document text](#randomize-document-text)
  * [Batch Templating](#batch-templating)
    + [Replacement Pre/Post-Markers](#replacement-prepost-markers)
  * [Arbitrary manual and scripted anlysis / modification](#arbitrary-manual-and-scripted-analysis--modification)  
  * [Output docxBox help or version number](#output-docxbox-help-or-version-number)  
* [Build Instructions](#build-instructions)
* [Running Tests](#running-tests)
* [Code Convention](#code-convention)
* [Changelog](#changelog)
* [Roadmap](#roadmap)
* [Bug Reporting and Feature Requests](#bug-reporting-and-feature-requests)
* [Third Party References](#third-party-references)
* [License](#license)


Commands
--------

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

**Compress XML, than zip files into DOCX:**

When having indented XML 
(i.e. via [``uzi``](#unzip-all-files-and-indent-xml-files) command) for manual 
manipulation, the ``zpc`` command compresses (= unindents) all XML files before 
zipping them into a new DOCX:

````docxbox zpc path/to/directory out.docx````  


### Output DOCX contents

#### List files: All, filtered, images only

**Lists files** contained within a given DOCX, and their attributes:

````docxbox ls foo.docx````

![List contents of DOCX archive](https://raw.githubusercontent.com/gyselroth/docxbox/master/screenshots/docxbox-docx-ls.png)

To output as JSON:

````docxbox lsj foo.docx````  
or ````docxbox ls foo.docx -j````  
or ````docxbox ls foo.docx --json````  

##### Filter by file ending

````docxbox ls foo.docx *.xml```` Lists only files ending w/ ``.xml`` 

##### List all files containing string or regular expression

````docxbox lsl foo.docx foo```` Lists all files containing the string ``foo``  
or ````docxbox ls foo.docx -l foo````  
or ````docxbox ls foo.docx --locate foo````  

This command is a shorthand to the grep tool (must be installed on your system 
when using this command).  
The search-string therefor can also be given as a regular expression:

````docxbox lsl foo.docx '/[0-9A-Z]\{8\}/'````  
Lists all files containing 8-digit IDs, e.g. word recent session IDs 
(ISO/IEC 29500-1). 

##### List all files containing string or regular expression as JSON

````docxbox lslj foo.docx foo````  
or ````docxbox lsl foo.docx -j foo````  
or ````docxbox ls foo.docx -lj foo````  
or ````docxbox lsl foo.docx --json foo````  
or ````docxbox ls foo.docx --locate -j foo````  
or ````docxbox ls foo.docx --locate --json foo````  


##### List image files

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


#### List meta data

docxBox displays only attributes that are contained within the current 
DOCX file (the attributes can vary by DOCX version and word processor used for 
creation), also if given empty.

Output meta data of given DOCX:  

````docxbox lsm foo.docx````  
or ````docxbox ls foo.docx -m````  
or ````docxbox ls foo.docx --meta````

![List document meta data](https://raw.githubusercontent.com/gyselroth/docxbox/master/screenshots/docxbox-docx-lsm.png)

To output as JSON:

````docxbox lsmj foo.docx````  
or ````docxbox lsm foo.docx -j````  
or ````docxbox ls foo.docx -mj````  
or ````docxbox lsm foo.docx --json````  
or ````docxbox ls foo.docx --meta --json````    

  
#### Reference: Recognized meta attributes

* **Authors:** Creator, lastModifiedBy (``<dc:creator>`` 
  and ``<cp:lastModifiedBy>`` of core.xml)
* **Dates** (ISO 8601): Creation-, modification and print-date  
  (``<dcterms:created>`` and ``<cp:modified>`` and ``<cp:lastPrinted>`` 
  of core.xml) 
* **Descriptions:** Description, Keywords, Subject, Title   
  (``<dc:description>``, ``<dc:keywords>``, ``<dc:subject>``, ``<dc:title>`` 
  of core.xml)
* **Language** (``<dc:language>`` of core.xml) 
* **Revision** (``<cp:revision>`` of core.xml)
* **Application** created with and its version, name of used template, company,
  XML schema of document (``<Application>``, ``<AppVersion>``, ``<Template>``, 
  ``<Properties xmlns ...`` and ``<Company>`` of app.xml)


#### List referenced fonts

````docxbox lsf foo.docx````  
or ````docxbox ls foo.docx -f````  
or ````docxbox ls foo.docx --fonts````  

![List referenced fonts](https://raw.githubusercontent.com/gyselroth/docxbox/master/screenshots/docxbox-docx-lsf.png)

To output as JSON:

````docxbox lsfj foo.docx````  
or ````docxbox lsf foo.docx -j````
or ````docxbox ls foo.docx -fj````  
or ````docxbox lsf foo.docx --json````  
or ````docxbox ls foo.docx --fonts --json````


#### List fields

````docxbox lsd foo.docx````  
or ````docxbox ls foo.docx -d````  
or ````docxbox ls foo.docx --fields````  

To output as JSON:  

````docxbox lsdj foo.docx````  
or ````docxbox ls foo.docx -dj````  
or ````docxbox lsd foo.docx --json````  
or ````docxbox ls foo.docx --fields --json````  


#### Output XML

````docxbox cat foo.docx word/_rels/document.xml.rels````  
outputs the given file's XML, indented for better readability.

**Hint:** For viewing or editing complex XML, e.g. with syntax highlightning, 
you can use your favorite text editor via the 
[``cmd`` command](#arbitrary-manual-and-scripted-analysis--modification)


#### Output document as plaintext

````docxbox txt foo.docx```` outputs the given document's plaintext
(ATM: w/o header and footer)

**Output plaintext segments:**  
````docxbox txt foo.docx -s````   
or ````docxbox txt foo.docx --segments````   

Outputs the plaintext from document, with markup sections separated by newlines.
This can be helpful to identify "segmented" sentences:
Texts which visually appear as a unit, but are declared within multiple separate
XML elements (due to formatting or change-tracking purposes).


### Compare DOCX documents

docxBox helps tracing changes to the files contained within DOCX archives,
made when manipulating documents in word processor applications.

When given two DOCX files, the ``ls`` command lists all files of both DOCX
documents side-by-side. docxBox compares all files and highlights files 
w/ different attributes or (identical attributes but) different content. 

````docxbox ls foo_v1.docx foo_v2.docx````  

![Compare two documents](https://raw.githubusercontent.com/gyselroth/docxbox/master/screenshots/docxbox-docx-ls-compare.png)

#### Compare specific file from two DOCX archives

Files that have changed between versions of a document, can be inspected using 
the ``diff`` tool (which must be installed on your system).  
 
Display **side-by-side** comparison of the formatted XML of given file 
(``word/settings.xml``), with differences indicated:  
````docxbox diff foo_v1.docx foo_v2.docx word/settings.xml````  

Display **unified** diff:
````docxbox diff foo_v1.docx foo_v2.docx word/settings.xml -u````  
or: ````docxbox diff foo_v1.docx foo_v2.docx word/settings.xml --unified````

![Compare file from two documents](https://raw.githubusercontent.com/gyselroth/docxbox/master/screenshots/docxbox-docx-diff.png)


### Modify document

#### Modify meta data

docxBox allows to modify existing attributes, or adds attributes if not present.

* Set **creation-date**:            ````docxbox mm foo.docx created "2020-01-29T09:21:00Z"````
* Set **creator** attribute:        ````docxbox mm foo.docx creator "docxBox v0.0.1"````  
* Set **description** attribute:    ````docxbox mm foo.docx description "Foo bar baz"````  
* Set **keywords** attribute:       ````docxbox mm foo.docx keywords "Foo bar baz"````  
* Set **language** attribute:       ````docxbox mm foo.docx language "en-US"````  
* Set **lastModifiedBy** attribute: ````docxbox mm foo.docx lastModifiedBy "docxBox v0.0.1"````
* Set **lastPrinted** attribute:    ````docxbox mm foo.docx lastPrinted "2020-01-10T10:31:00Z"````
* Set **modification-date**:        ````docxbox mm foo.docx modified "2020-01-29T09:21:00Z"````
* Set **revision** attribute:       ````docxbox mm foo.docx revision 2````
* Set **subject** attribute:        ````docxbox mm foo.docx subject "Foo bar"````
* Set **title** attribute:          ````docxbox mm foo.docx title "Foo bar, baz"````

**Note:** Altering meta data does NOT automatically update preview texts of
generic fields, which display respective meta data.  
For updating field values, use the [``sfv``](#set-field-value-merge-fields-generic-fields) 
command.

To alter/insert an attribute and save the modified document to a new file:  
````docxbox mm foo.docx <attribute> <value> new.docx````


#### Replace image

````docxbox rpi foo.docx image1.jpeg /home/replacement.jpeg```` overwrites the 
DOCX w/ the modified document.

**Note:** The original and replacement image must be of the same format 
(bmp, gif, jpg, etc.).

````docxbox rpi foo.docx image1.jpeg /home/replacement.jpeg new.docx````  
This creates a new file: new.docx


#### Replace text

Replace all (case-sensitive) occurrences of given string in DOCX text:

````docxbox rpt foo.docx old new```` updates foo.docx  
````docxbox rpt foo.docx old new new.docx```` creates a new file new.docx  


#### Replace by markup

Moreover [replacing text](#replace-text) and 
[fields](#set-field-value-merge-fields-generic-fields), docxBox supports 
rendering and inserting the following 
[Office Open XML](https://en.wikipedia.org/wiki/Office_Open_XML) elements:

* [Heading 1, 2, 3](#insert-heading)
* [Text](#insert-text)
* [Paragraph containing text](#insert-paragraph-containing-text)
* [Image](#insert-image) (formats: ``bmp``, ``emg``, ``gif``, ``jpeg``, 
  ``jpg``, ``png``, ``tif``, ``tiff``, ``wmf``)
* [Table](#insert-table)
* [Ordered list, unordered list](#insert-list)  

Markup specification for such elements must be given as JSON, following these
rules: 

* JSON must be wrapped within ``{...}``
* The first item must be a type identifier (``h1``, ``h2``, ``h3``,
  ``img`` / ``image``, ``ol``, ``table``, ``ul``)
* All attributes are given associative (as JSON object related to the type) 
* The order of attributes within the config of the type is arbitrary


##### Insert heading

**Example:** Replace string ``search`` by a Heading 1 with the text 
``Foo``:  
````docxbox rpt foo.docx search "{\"h1\":{\"text\":\"Foo\"}}"````  

docxBox supports rendering of Header 1, 2 and 3 (``h1``, ``h2``, ``h3``).


##### Insert text

**Example:** Replace string ``search`` (by a new run) with the text 
``Foo``:  
````docxbox rpt foo.docx search "{\"text\":{\"text\":\"Foo\"}}"````  


##### Insert paragraph containing text

**Example:** Replace string ``search`` (by a new paragraph containing a run) 
with the text ``Foo``:  
````docxbox rpt foo.docx search "{\"p\":{\"text\":\"Foo\"}}"````  


##### Insert hyperlink

**Example:** Replace string ``search`` by a hyperlink:    
````docxbox rpt foo.docx search "{\"link\":{\"text\":\"docxBox\",\"url\":\"https://github.com/gyselroth/docxbox\"}}"````  


##### Insert list

Replace string ``search`` by an unordered list:  
````docxbox rpt foo.docx search "{\"ul\":{\"items\":[\"item-1\",\"item-2\",\"item-3\"]}}"````  

Replace string ``search`` by an ordered list:  
````docxbox rpt foo.docx search "{\"ol\":{\"items\":[\"item-1\",\"item-2\",\"item-3\"]}}"````


##### Insert image

**Image markup specification example:** 

````
{
    "img":{
        "name":"example.jpg",
        "offset":[0,0],
        "size":[2438400,1828800]
    }
}
````

**Specification rules:**

* The ``name`` parameter is optional
* The ``offset`` argument is optional
* Size is given in EMUs (English Metric Unit) that is: ``pixels * 9525``

When inserting a new image file, it must be given as additional argument:  
````docxbox rpt foo.docx search "{\"image\":{\"size\":[2438400,1828800]}}" images/ex1.jpg````


##### Insert table

To replace text by a newly rendered table like:

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

* ``header`` is optional, when given: ``columns`` is optional
* ``content`` is optional, when given: ``rows`` is optional

Replace ``search`` by table:    
````docxbox rpt foo.docx search "{\"table\":{\"header\":[\"A\",\"B\",\"C\"],\"content\":[[\"a1\",\"a2\",\"a3\"],[\"b1\",\"b2\",\"b3\"],[\"c1\",\"c2\",\"c3\"]]}}"````


#### Remove content between text

Remove content between (and including) given strings (``left`` and ``right``):    

````docxbox rmt foo.docx left right```` updates foo.docx  
````docxbox rmt foo.docx left right new.docx```` creates a new file new.docx


#### Set field value: Merge fields, generic fields

##### Merge fields

When setting the value (text) of a **merge field**, the merge field is reduced 
to its textual component (maintaining its visual style).
  
**Note:** A particular merge field can NOT be merged repeatedly: 
merging turns the former field into a text element (the field subsequently 
does not exist as such any more).  

````docxbox sfv foo.docx "MERGEFIELD  foo" bar```` (Updates ``foo.docx``)  
Changes all merge fields, whose identifier begins with ``foo``, 
into the text ``bar``.  

````docxbox sfv foo.docx "MERGEFIELD  foo" bar new.docx````
Saves the resulting DOCX to a new DOCX file: ``new.docx``

**Hint:** To find out field identifiers use docxBox's [``lsd``](#list-fields)
command.

##### Generic fields

Setting field values includes also preview texts of otherwise
**generic fields**, which in some word processing applications have to be
updated explicitly.

````docxbox sfv foo.docx "PRINTDATE" "10.01.2020"````  
Updates the shown text of all print-date fields to ``10.01.2020``.


#### Randomize document text

Replace all text of an existing document by similarly structured random 
"Lorem Ipsum" dummy text, helpful for generating DOCX documents for testing 
purposes:

````docxbox lorem foo.docx```` updates foo.docx  
````docxbox lorem foo.docx new.docx```` creates a new file new.docx  


### Batch Templating

docxBox's batch templating mode allows to perform an arbitrary sequence of 
operations (supporting all docxBox commands for document manipulation) upon 
a given DOCX. It thereby facilitates a more extensive range of templating
options than the commands directly (= without batch templating) available.  

**Example:** docxBox does not directly support replacing merge fields by other
than plain textual content. Via batch templating, merge fields can be 
transformed into text in one step of a sequence, which can completely or in 
part, in a later step be replaced by generic content like for example a table, 
which can later be filled with more content.  


#### Replacement Pre/Post Markers
Batch templating can make use of "markers": optional text elements containing a 
distinct identifier string. Markers can temporarily be inserted and can 
subsequently be replaced again at a later step of the batch sequence by other 
generic content. 

**Rules:**
* Markers can be added **before** (key: ``pre``) and **after** (key: ``post``) 
  the actual generic replacement content
* Markers can either be of the type ``text`` or ``paragraph`` (or ``p``) 
  to insert surrounding line-breaks
* Markers contain a textual identifier, which can use any text (but should be 
distinct within the document)

##### Batch sequence JSON

Sequences of templating steps to be batch-processed must be given like:   
````
{
 "<STEP_ID>": {"<COMMAND>": [("<ARGUMENT_1>",)(,"<ARGUMENT_2>",...)]},
 "<STEP_ID>": {"<COMMAND>": [("<ARGUMENT_1>",)(,"<ARGUMENT_2>",...)]},
 ...
}
````

**Example:**
````
{
 "1": {"mm": ["description", "foo"]},
 "2": {"rpt": ["bar", "baz"]},
 "3": {"rpt": [
    "qux", 
    {"h1": {"text": "Quux"}}
 ]}
}
````

**Rules:**
* Every step must be given as a tuple of step-ID and -parameters
* ``<STEP_ID>`` is an arbitrary string, must be distinct within the sequence
* Parameters must be given as a tuple of a command and its respective arguments
* ``<COMMAND>`` accepts any of docxBox's commands for DOCX manipulation 
  ([``rmt``](#remove-content-between-text), [``rpi``](#replace-image), 
  [``rpt``](#replace-text), [``lorem``](#randomize-document-text), 
  [``mm``](#modify-meta-data) and 
  [``sfv``](#set-field-value-merge-fields-generic-fields))
* ``<ARGUMENT>``: Argument(s) for respective command, same as in non-batch mode
* When a command has no arguments (e.g. [``lorem``](#randomize-document-text)), 
  an empty array must be given though (E.g.: ``{"lorem":[]}``)
* Arguments for markup-configuration of generic document elements can be given 
  as nested JSON   


##### Example: Replace string by heading-1 followed by table containing images 

**Templating sequence:**

1. **Step "1":** Replace string ``foo`` by heading-1 with the text: ``Foobar``
   (followed by a temporary marker ``my-marker-1``)
2. **Step "2":** Replace the marker ``my-marker-1`` by table containing 2x2 
   cells 
3. **Steps "3" to "6":** Replace (the placeholder texts within the) table cells 
   by images
4. Add new image files into docx document

**Batch config:**
````
{
 "1": {"rpt": [
    "foo",
    {
     "h1": {
        "text": "Foobar",
        "post": {"text": "my-marker-1"}
     }
    }
 ]},
 "2": {"rpt": [
   "my-marker-1",
   {
     "table": {
         "columns": 2,
         "rows": 2,
         "header": ["A","B"],
         "content": [
             ["img-a1", "img-b1"],
             ["img-a2", "img-b2"]
         ]
     }
   }  
 ]},
 "3": {"rpt": [
    "img-a1",
    {
      "img": {
          "name": "blue.png",
          "size": [2438400, 1828800]
      }
    }  
 ]},
 "4": {"rpt": [
    "img-b1",
    {
      "img": {
          "name": "green.png",
          "size": [2438400, 1828800]
      }
    }
 ]},
 "5": {
    "rpt": [
      "img-a2",
      {
        "img":{
          "name": "orange.png",
          "size": [2438400, 1828800]
        }
      }
 ]},
 "6": {"rpt": [
    "img-b2",
    {
      "img": {
          "name": "red.png",
          "size": [2438400,1828800]
      }
    }
 ]}  
}
````

**The full batch command:**

**Note:** As when inserting new images in non-batch mode 
(via [``rpt``](#insert-image) or [``rpi``](#replace-image)), also during batch
templating, image files to be added into the document must be given as trailing 
arguments.
 
````docxbox batch foo.docx "{\"1\":{\"rpt\":[\"foo\",{\"h1\":{\"text\":\"Foobar\",\"post\":{\"text\":\"my-marker-1\"}}}]},\"2\":{\"rpt\":[\"my-marker-1\",{\"table\":{\"columns\":2,\"rows\":2,\"header\":[\"A\",\"B\"],\"content\":[[\"img-a1\",\"img-b1\"],[\"img-a2\",\"img-b2\"]]}}]},\"3\":{\"rpt\":[\"img-a1\",{\"img\":{\"name\":\"blue.png\",\"size\":[2438400,1828800]}}]},\"4\":{\"rpt\":[\"img-b1\",{\"img\":{\"name\":\"green.png\",\"size\":[2438400,1828800]}}]},\"5\":{\"rpt\":[\"img-a2\",{\"img\":{\"name\":\"orange.png\",\"size\":[2438400,1828800]}}]},\"6\":{\"rpt\":[\"img-b2\",{\"img\":{\"name\":\"red.png\",\"size\":[2438400,1828800]}}]}}" blue.png green.png orange.png red.png````


##### Save batch processed document to new file

To save the resulting document of batch processed manipulations to a new file, 
instead of overwriting the source document, the destination filename can 
optionally be given as the very last argument (also trailing other optional 
arguments like image files):

````docxbox batch foo.docx "{\"1\":{\"mm\":[\"description\",\"foo\"]},\"2\":{\"rpt\":[\"bar\",\"baz\"]},\"3\":{\"rpt\":[\"qux\",{\"h1\":{\"text\":\"Quux\"}}]}}" new.docx```` 


### Arbitrary manual and scripted analysis / modification

docxBox eases conducting arbitrary modifications on files contained within a 
DOCX, manually and scripted.  
All steps besides the actual modification are automated via docxBox, with the 
respective user-defined modification inserted.

**Example - Edit XML file manually:**

````docxbox cmd foo.docx "nano *DOCX*/word/document.xml"````  

docxBox in the above example does:
1. **Unzip** ``foo.docx``
2. **Indent** all extracted XML files
3. Render (= replace ``*DOCX*`` w/ the resp. extraction path)  
  and **execute** the command: ``nano *DOCX*/word/document.xml``, 
  thereby opening ``document.xml`` for editing in nano, halting docxBox until 
  exiting the editor.
4. **Unindent** all extracted XML files
5. **Zip** the extracted files back into ``foo.docx``


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

In order to run functional tests, 
[Bats](https://github.com/sstephenson/bats) must be installed.

Run all tests: `./test.sh`


Code Convention
---------------

The source code of docxBox follows the 
[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).  
The source code of functional tests follows the 
[Google Shell Style Guide](https://google.github.io/styleguide/shell.xml)


Changelog
---------

See [Changelog](https://github.com/gyselroth/docxbox/blob/master/CHANGELOG.md)


Roadmap
-------

* v0.0.5: Batch process sequences of manipulation operations
* v0.0.6: Ensure microsoft word compatibility
* v0.1.0: Add optional configuration options via environment vars
* v0.1.0: (Optional) logging of operations
* v1.0.0: Libre-Office compatible appending of two DOCX files into a single one 
  (by XML appending, instead of adding sub-documents) 


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
| [Clang](https://clang.llvm.org)                                 | A C language family frontend for LLVM                                      | [Apache License](https://www.apache.org/licenses/)                                                        |      
| [Cmake](https://cmake.org)                                      | Family of tools designed to build, test and package software               | [New BSD License](https://en.wikipedia.org/wiki/New_BSD_License)                                          |      
| [Cppcheck](http://cppcheck.sourceforge.net/)                    | Static analysis tool for C/C++ code                                        | [GNU General Public License version 3](https://www.gnu.org/licenses/gpl-3.0.html)                         |      
| [cpplint](https://github.com/cpplint/cpplint)                   | Static code checker for C++                                                | [BSD-3 Clause](https://opensource.org/licenses/BSD-3-Clause)                                              |      
| [GCC](https://gcc.gnu.org)                                      | GCC, the GNU Compiler Collection                                           | [GNU General Public License version 3](https://gcc.gnu.org/onlinedocs/libstdc++/manual/appendix_gpl.html) |      
| [ShellCheck](https://github.com/koalaman/shellcheck)            | A static analysis tool for shell scripts                              | [GNU General Public License version 3](https://gcc.gnu.org/onlinedocs/libstdc++/manual/appendix_gpl.html) |      
| [Travis CI](https://travis-ci.org)                              | Hosted Continuous Integration Service                                      | [MIT License](https://opensource.org/licenses/MIT)                                                        |
| [Valgrind](https://valgrind.org)                                | System for debugging and profiling Linux programs                          | [GNU General Public License, version 2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html)           |

Thanks a lot!


License
-------

docxBox is licensed under
[The MIT License (MIT)](https://opensource.org/licenses/MIT)
