import sys

class Type:
    def __init__(self, cpp, cs, default, length):
        self.mCpp = cpp
        self.mCs = cs
        self.mDefault = default
        self.mLength = length

types = {"int8" : Type("int8_t", "sbyte", "0", 1), 
         "uint8" : Type("uint8_t", "byte", "0", 1),
         "int16" : Type("int16_t", "Int16", "0", 2),
         "uint16" : Type("uint16_t", "UInt16", "0", 2),
         "int32" : Type("int32_t", "Int32", "0", 4),
         "uint32" : Type("uint32_t", "UInt32", "0", 4),
         "int64" : Type("int64_t", "Int64", "0", 8),
         "uint64" : Type("uint64_t", "UInt64", "0", 8),
         "float" : Type("float", "float", "0.0f", 4),
         "double" : Type("double", "double", "0.0", 8),
         "char" : Type("char", "char", "\' \'", 1)}

enums = {}
packets = {}

class Enum:
    def __init__(self):
        self.mMembers = {}
        self.mMemberKeys = []
        self.mName = ""
        self.mType = "int32"
    
    def parsing(self, tokens, index):
        self.mName = tokens[index]
        if(tokens[index+1] == ":"):
            if(tokens[index+2] in types):
                self.mType = tokens[index+2]
            else:
                print tokens[index+2] + "is invalid type."
            
            index = index + 2
        
        index = index + 1
        
        if tokens[index] != "{":
            print "invalid syntax. expected \`{\` omitted."
        
        index = index + 1
        value = 0
        while index < len(tokens):
            if(tokens[index] == "}"):
                return index + 1
            
            member = tokens[index]
            self.mMemberKeys.append(member) 
            
            if(tokens[index+1] == "="):
                try:
                    value = int(tokens[index+2])       
                except ValueError:
                    print "invalid number." + tokens[index+2] + " is not a number."
                    sys.exit()
                index = index + 2
            
            self.mMembers[member] = value
            
            index = index + 1
            value = value + 1
        
        print "invalid syntax. expected \`}\` omitted."
        sys.exit()
        
    def toString(self, lang):
        s = ""
        if(lang == 0): # cpp
            s = s + "enum class " + self.mName + " : " + types[self.mType].mCpp + "\n"
        elif(lang == 1): # cs
            s = s + "public enum " + self.mName + "\n"
            
        s = s + "{\n"
        
        for key in self.mMemberKeys:
            s= s + "\t" + key + " = " + str(self.mMembers[key]) + ",\n"
        
        if(lang == 0): # cpp
            s = s + "};\n"
        elif (lang == 1): # cs
            s = s +"}\n"
        
        return s
        
class Member:
    def __init__(self):
        self.mType = ""
        self.mName = ""
        self.mDefault = ""
        self.mLength = 0
        self.mNum = 0
    
    def parsing(self, tokens, index):
        if tokens[index] in types:
            self.mType = tokens[index]
            self.mLength = types[tokens[index]].mLength
        elif tokens[index] in enums:
            self.mType = enums[tokens[index]].mName
            self.mLength = types[enums[tokens[index]].mType].mLength
        elif tokens[index][:6] == "string":
            self.mType = "string"
            try:
                self.mLength = int(tokens[index][6:])
            except ValueError:
                print tokens[index][6:] + " is not a number."
                sys.exit()
        else:
            print tokens[index] + " is invalid type."
            sys.exit()
        
        self.mName = tokens[index+1]
        index = index + 2
        
        if(tokens[index] == "="):
            self.mDefault = tokens[index+1]
            index = index + 2
        elif(tokens[index] == "arr"):
            self.mNum = int(tokens[index+1])
            index = index + 2
        
        return index
        
    def toString(self, langType):
        s = "\t"
        if langType == 0: # cpp
            if self.mType == "string":
                s = s + "char " + self.mName + "[" + str(self.mLength) + "]"
            elif self.mType in types:
                s = s + types[self.mType].mCpp + " " + self.mName
            elif self.mType in enums:
                s = s + enums[self.mType].mName + " " + self.mName
            
            if self.mNum != 0:
                s = s + "[" + str(self.mNum) + "]"
            
            if self.mDefault != "":
                s = s + " = " + self.mDefault
        
            s = s + ";\n"
        elif langType == 1: # cs
            if self.mNum != 0:
                s = s + "[MarshalAs(UnManagedType.ByValArray, SizeConst = " + str(self.mNum) + ")]\n\t"
                if self.mType in types:
                    s = s + "public " + types[self.mType].mCs + "[] " + self.mName
                else:
                    s = s + "public " + types[enums[self.mType].mType].mCs + "[] " + self.mName
            elif self.mType == "string":
                s = s + "[MarshalAs(UnmanagedType.ByValArray, SizeConst = " + str(self.mLength) + ")]\n\t"
                s = s + "public char[] " + self.mName
            elif self.mType in types:
                s = s + "[MarshalAs(UnmanagedType.U" + str(self.mLength) + ")]\n\t"
                s = s + "public " + types[self.mType].mCs + " " + self.mName
            elif self.mType in enums:
                s = s + "[MarshalAs(UnmanagedType.U" + str(self.mLength) + ")]\n\t"
                s = s + "public " + types[enums[self.mType].mType].mCs + " " + self.mName
            
            
            
            if self.mDefault != "":
                s = s + " = " + self.mDefault
            
            s = s + ";\n"
        return s

class Packet:
    def __init__(self):
        self.mMembers = {}
        self.mMemberKeys = []
        self.mName = ""
        self.mBase = ""
        self.mLength = 0
    
    def parsing(self, tokens, index):
        self.mName = tokens[index]
        if(tokens[index+1] == ":"): # base class
            self.mBase = tokens[index+2]
            index = index + 2
            
        index = index + 1
        
        if tokens[index] != "{":
            print "invalid syntax. expected \`{\` omitted."
            
        index = index + 1

        while index < len(tokens):
            if(tokens[index] == "}"):
                return index + 1
            
            newMember = Member()
            index = newMember.parsing(tokens,index)
            self.mMembers[newMember.mName] = newMember
            self.mMemberKeys.append(newMember.mName)
            self.mLength += newMember.mLength
        
        print "invalid syntax. expected \`}\` omitted."
        sys.exit()
    
    def toString(self, langType):
        s = ""
        if langType == 0: #cpp
            s = s + "#pragma pack(push, 1)\n"
            s = s + "struct " + self.mName
            if self.mBase != "":
                s = s + " : " + self.mBase
        
            s = s + "\n{\n"
            
            for key in self.mMemberKeys:
                s = s + self.mMembers[key].toString(langType)
            
            s = s + "};\n"
            s = s + "#pragma pack(pop)\n"
        elif langType == 1: #cs
            s = s + "[StructLayout(LayoutKind.Sequential, Pack = 1)]\n"
            s = s + "public class " + self.mName
            if self.mBase != "":
                s = s + " : " + self.mBase
            
            s = s + "\n{\n"
            for key in self.mMemberKeys:
                s = s + self.mMembers[key].toString(langType)
                
            s = s + "}\n"
        
        return s

def tokenize(str, delims, rangeDelims):
    tokens = []
    isInRange = False
    rangeStart = -1
    
    for i in range(0, len(str)):
        if isInRange:
            if str[i] == str[rangeStart]:
                tokens.append(str[rangeStart+1:i])
                isInRange = False
                rangeStart = i
        else:
            if str[i] in rangeDelims:
                rangeStart = i
                isInRange = True
            elif str[i] in delims:
                if rangeStart + 1 != i:
                    tokens.append(str[rangeStart+1:i])
                rangeStart = i
                
    tokens.append(str[rangeStart+1:])

    return tokens

def beginString(langType):
    if langType == 0: #cpp
        return "#pragma once\n#include <stdint.h>\n\n"
    elif langType == 1: #cs
        return "using System;\nusing System.Text;\nusing System.Runtime.InteropServices;\n\nnamespace Packet\n{\n\n"
    else:
        return ""

def endString(langType):
    if langType == 0: #cpp
        return ""
    elif langType == 1: #cs
        return "}\n"
    else:
        return ""

print "generate..."

packet = ""
packetList = []
lang = sys.argv[1]
langType = -1
if lang == "cpp":
    langType = 0
elif lang == "cs":
    langType = 1
else:
    print "invalid lang."
    sys.exit()
    
src = sys.argv[2]
dest = sys.argv[3]

with open(src, 'r') as list:
    data = list.read()
    tokens = tokenize(data, "\n\t ", "\"")
    tokenIndex = 0
    
    while tokenIndex < len(tokens):
        if(tokens[tokenIndex] == "enum"):
            newEnum = Enum()
            tokenIndex = newEnum.parsing(tokens, tokenIndex + 1)
            enums[newEnum.mName] = newEnum
        elif (tokens[tokenIndex] == "packet"):
            newPacket = Packet()
            tokenIndex = newPacket.parsing(tokens, tokenIndex + 1)
            packets[newPacket.mName] = newPacket
            packetList.append(newPacket)
        else:
            print tokens[tokenIndex] + " is invalid keyword."
            sys.exit()
    
out = beginString(langType)
for enum in enums.values():
    out = out + enum.toString(langType)

for packet in packetList:
    out = out + packets[packet.mName].toString(langType)

out = out + endString(langType)
    
with open(dest, 'w') as outFile:
    outFile.write(out)
    

print "end..."