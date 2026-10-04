import csv
with open('resultsTest.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        print(row['cols'], row['checksum'])