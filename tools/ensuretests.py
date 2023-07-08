#! /usr/bin/env python3

#
# Python script to check the Fangorn project and ensure all classes have tests created for them.
#

import os


#prompts the user to select a number of options
def prompt(msg: str, options: 'list[str]'):
    print(msg)
    for i in range(0, len(options)):
        print("    {}: {}".format(i + 1, options[i]))
    
    response = input("[{}-{}]? ".format(1, len(options)))
    while not response.isnumeric() or int(response) < 1 or int(response) > len(options):
        response = input("Invalid response: please enter a number from {} to {}: ".format(1, len(options)))
    
    return int(response) - 1


#returns the contents of a specified file as a single string.
def readFileToString(name: str):
    f = open(name, 'r')
    contents = "".join(f.readlines())
    f.close()
    
    return contents


#writes the provided string to the file with the provided name.
def writeStringToFile(name: str, contents: str):
    f = open(name, 'w')
    f.write(contents)
    f.close()


#reads the template file at templatePath, subs in the args, and writes the file to targetPath.
#args are handled as such. Any <1>'s in the template file will be replaced by the 0th arg, <2>'s are replaced by the 1st arg, and so on.
def createFileFromTemplate(templatePath: str, targetPath: str, args: 'list[str]'):
    contents = readFileToString(templatePath)
    
    for i in range(0, len(args)):
        contents = contents.replace("<{}>".format(i + 1), args[i])
    
    writeStringToFile(targetPath, contents)


#subtracts src from dst and returns dst
def subtractLists(dst: 'list[str]', src: 'list[str]'):
    for srcItem in src:
        if srcItem in dst:
            dst.remove(srcItem)
    
    return dst


def resolveClassNames(filenames: 'list[str]'):
    classNames = []
    
    for filename in filenames:        
        if filename.endswith('.cpp'):
            filename = filename[ : len(filename) - 4]
            if filename.startswith("Test"):
                filename = filename[4 : ] #shave off "Test"
            
            classNames.append(filename)
    
    return classNames
    

#ensures proper test configuration between srcDir and testDir
def ensureTestsForCategory(projectRoot: str, category: str):
    print(f"Checking project category {category}")
    srcDir = os.path.join(projectRoot, "src", "fangorn", category)
    testDir = os.path.join(projectRoot, "test", "fangorn", category)
    
    srcFiles = resolveClassNames(os.listdir(srcDir))
    testFiles = resolveClassNames(os.listdir(testDir))
    
    uniqueSrcNames = subtractLists(srcFiles, testFiles)
    uniqueTestNames = subtractLists(testFiles, srcFiles)
    
    print(f"Unique sources: {uniqueSrcNames}")
    print(f"Unique tests: {uniqueTestNames}")
    
    testTemplateFile = os.path.join(__file__, "templates", "test_template")
    emptyTemplateFile = os.path.join(__file__, "templates", "empty_file")
    
    for name in uniqueSrcNames:
        resp = prompt(
            f"Class {name} does not have a test. What would you like to do?",
            [
                "Ignore the issue",
                "Create a test",
                "Delete the source"
            ]
        )
        
        if resp == 1:
            #create the test
            createFileFromTemplate(testTemplateFile, os.path.join(testDir, f"Test{name}.cpp"), [name])
        
        if resp == 2:
            #delete the source
            os.remove(os.path.join(srcDir, f"{name}.cpp"))
    
    for name in uniqueTestNames:
        resp = prompt(
            f"Class {name} does not have a source. What would you like to do?",
            [
                "Ignore the issue",
                "Create a source",
                "Delete the test"
            ]
        )
        
        if resp == 1:
            #create the source
            createFileFromTemplate(emptyTemplateFile, os.path.join(srcDir, f"{name}.cpp", []))
        
        if resp == 2:
            #delete the test
            os.remove(os.path.join(testDir, f"Test{name}.cpp"))
            
        

def main():
    currFile = os.path.abspath(__file__).replace('\\', '/')
    projectRoot = currFile[0 : currFile.rfind('tools/')]
    
    print(f"Project root: {projectRoot}")
    
    ensureTestsForCategory(projectRoot, "control")
    ensureTestsForCategory(projectRoot, "model")
    ensureTestsForCategory(projectRoot, "util")


if __name__ == "__main__":
    main()
