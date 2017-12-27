import random

def main():
    with open('intsLarge.txt','w+') as f:
        for i in range(0,10000):
            f.write(str(random.uniform(1,99999)))
            f.write("\n")



if __name__ == '__main__':
    main()