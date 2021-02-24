import os
import subprocess
from subprocess import PIPE

class Assertion:
    def __init__(self, folder_path):
        self.folder_path = folder_path
    
    def assert_all(self):
        print("Assert start")
        for dirpath, dirnames, filenames in os.walk(self.folder_path):
            print(f"[{dirpath}]")
            for filename in filenames:
                filepath = os.path.join(dirpath, filename)
                root, ext = os.path.splitext(filepath)
                if ext == ".c":
                    compath = f"{root}.txt"
                    respath = f"{root}_result.txt"
                    self.assert_file(filepath, compath, respath)
        print("All complete!")

    def assert_file(self, filepath, compath, respath):
        filename = os.path.basename(filepath)
        print(f"{filename} => ", end="")
        self.preprocessing(filepath, respath)
        res = self.read_file(respath)
        com = self.read_file(compath)
        self.assert_result(res, com)

        
    def preprocessing(self, filepath, respath):
        code = "".join(self.read_file(filepath))

        command = ["./9cc", code]
        proc = subprocess.run(command, stdout=PIPE, stderr=PIPE, text=True)
        self.write_file("tmp.s", proc.stdout)
        if proc.stderr:
            print("Compile error")
            print(proc.stderr)
            exit(1)

        command = "cc -o tmp tmp.s lib.o"
        proc = subprocess.run(command, shell=True, stderr=PIPE, text=True)
        if proc.stderr:
            print("Assemble error")
            print(proc.stderr)
            exit(1)

        command = "./tmp"
        proc = subprocess.run(command, shell=True, stdout=PIPE, stderr=PIPE, text=True)
        self.write_file(respath, proc.stdout)
        if proc.stderr:
            print("Runtime error")
            print(proc.stderr)
            exit(1)

    def read_file(self, path):
        with open(path) as file:
            text = [s.strip(os.linesep) for s in file.readlines()]
        return text

    def write_file(self, path, text):
        with open(path, mode='w+') as file:
            file.write(text)

    def assert_result(self, res, com):
        if len(res) != len(com):
            print("NG: The Number of lines is different.")
            print(f"result: {len(res)} ideal: {len(com)}")
            exit(1)
        
        for i in range(len(res)):
            if res[i] != com[i]:
                print("NG: The output result is different.")
                print(f"Line {i + 1}")
                print(f"result:")
                print(res[i])
                print(f"ideal:")
                print(com[i])
                exit(1)

        print("OK")

if __name__ == "__main__":
    folder_path = "/mnt/c/Users/Kosuke Futamata/source/compiler/9cc/test/"
    assertion = Assertion(folder_path)
    assertion.assert_all()