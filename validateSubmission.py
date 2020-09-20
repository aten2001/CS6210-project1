import sys
import zipfile
import json

projectJson = {
    "ProjectStructure": [
        "/cpu/vcpu_scheduler.c",
        "/cpu/Makefile",
        "/cpu/Readme",
        "/cpu/vcpu_scheduler1.log",
        "/cpu/vcpu_scheduler2.log",
        "/cpu/vcpu_scheduler3.log",
        "/cpu/vcpu_scheduler4.log",
        "/cpu/vcpu_scheduler5.log",
        "/memory/memory_coordinator.c",
        "/memory/Makefile",
        "/memory/Readme",
        "/memory/memory_coordinator1.log",
        "/memory/memory_coordinator2.log",
        "/memory/memory_coordinator3.log"
    ]
}

if __name__ == '__main__':
    file = sys.argv[1]
    try:
        zip = zipfile.ZipFile(file, 'r')
        submission_files = zip.namelist()
        submission_files = ["/" + f.split("/", 1)[1] for f in submission_files]
        project_files = projectJson["ProjectStructure"]
        check = all(item in submission_files for item in project_files)
        if check:
            print("Submission is Valid")
        else: 
            print("Invalid Submission Structure. Check Canvas instructions and make sure that all the files listed are present in your zip file")
    except FileNotFoundError or FileExistsError:
        print("File Not found")
    except Exception:
        print("Invalid Submission Structure. Check Canvas instructions and make sure that all the files listed are present in your zip file")
