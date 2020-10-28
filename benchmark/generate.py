#!/usr/bin/env python3
import re
from os import listdir
from os.path import isfile, join
files = [f for f in listdir(".") if isfile(join(".", f)) and ".cfg" in f]
template = open("template.c","r").read()
ymltext = open("template.yml","r").read()

dataset = dict()
def process(f, applyfix = False):
    data = dict()
    branchconds = list()
    branchcodes = list()
    mode = 0
    data["comment"] = "/* %s:\n" %f
    for line in  open(f,"r"):
        data["comment"] += line
        line = line.strip()
        parts = line.split("=",maxsplit=1)
        if not "branch" in parts[0]:
            data[parts[0]] = parts[1]
        else:
            if mode == 0:
                assert parts[0] == "branchcondition"
                branchconds.append(parts[1])
                mode = 1
            elif mode == 1:
                assert parts[0] == "branch"
                branchcodes.append(parts[1])
                mode = 0
    data["comment"] +="*/"
    category = data["learners"].split()[-1]
    if "conj" in category:
        category = "conjunctive"
    elif "disj" in category:
        category = "disjunctive"
    elif "poly" in category:
        category = "polynomial"
    basename = "benchmark"+f[:-4]+"_"+category
    data["file"] = basename+".c"
    if "loop" in data and data["loop"] != "":
        assert len(branchconds) == 0
        data["loop"] = re.sub("; ",";\n    ",data["loop"])
    elif len(branchconds) > 0:
        assert "loop" not in data or data["loop"] == ""
        assert len(branchconds) == len(branchcodes)
        body = ""
        start = ""
        for i in range(len(branchconds)):
            if branchconds[i] == "":
                body += " else {\n"
            else:
                body += start + "if (%s) {\n" % branchconds[i]
            body += "      %s\n" % branchcodes[i]
            body += "    }"
            start = "\n    "
        data["loop"] = body
    
    for (key,value) in data.items():
        data[key] = value.strip()
    
    variables = list()
    for key in ["beforeloop","beloopinit", "names"]:
        if key in data:
            entry = data[key]
            if "int " in entry and key != "names":
                for v in re.sub(";",",",re.sub("int ","",entry)).strip(",").split(","):
                    variables.append(v.strip())
            elif key == "names" and entry != "":
                for v in entry.strip().split():
                    variables.append(v.strip())
    
    assert len(set(variables)) == len(variables) 
    if len(variables)>0:
        result = ""
        for v in variables:
            result += "\n  int %s = __VERIFIER_nondet_int();" % v
        data["init"] = result
    else:
        data["init"] = ""

    # deal with symbolic variables (mostly "symbolic=flag")
    if "symbolic" in data:
        symvars = [entry.strip() for entry in data["symbolic"].split()]
        result = ""
        for v in symvars:
            result += "\n  _Bool %s = __VERIFIER_nondet_bool();" % v
        data["symbolicinit"] = result

    # indent some init code or set to "" if not present:
    for key in ["beforeloopinit","symbolicinit"]:
        if key not in data:
            data[key] = ""
        else:
            data[key] = "\n  " + data[key]

    # deal with preconditions: indent and encapuslate in if (!precond) return 0;
    # if there is no precondition, we set it to ""
    for key in ["precondition",]:
        if key not in data:
            data[key] = ""
        else:
            data[key] = "\n  if (!(%s)) return 0;" % data[key]
    if "loopcondition" in data and data["loopcondition"] == "":
        data["loopcondition"] = "__VERIFIER_nondet_bool()"
    

    # fill in all the parts into the template
    filledtemplate = template
    for (key,value) in data.items():
        #print("$"+key)
        filledtemplate = re.sub("\$\("+key+"\)",value,filledtemplate)
    
    filledyml = re.sub("\$\(file\)",basename+".i",ymltext)
    filledyml = re.sub("\$\(overflow_verdict\)","true",filledyml)
    open (basename+".c","w").write(filledtemplate)
    open (basename+".yml","w").write(filledyml)
    dataset[f] = data

for f in files:
    if f in ("f2.cfg","test.cfg"):
        continue

    process(f, applyfix = False)
    process(f, applyfix = True)
