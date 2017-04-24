import csv

with open('test.csv', 'ab') as csvfile:
    testwriter = csv.writer(csvfile, delimiter=',')
    testwriter.writerow(['a,b,c'])
    testwriter.writerow(['span']*5+['another'])
    csvfile.close()