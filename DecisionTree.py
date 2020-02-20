import math
import copy
import Queue as Q
pqueue = Q.PriorityQueue()
root = None

class DecisionTree:
    def __init__(self, featureSelection):

        #Average or information gain weighted
        self.featureSelection = featureSelection

        fh = open('trainData.txt', 'r')
        trainDataPoints = []
        trainDataPoints = fh.readlines()
        fh.close()

        fh = open ('words.txt', 'r')
        self.words = fh.readlines()
        fh.close()
        fh = open('trainLabel.txt', 'r')
        self.trainDocLabels = fh.readlines()
        fh.close()
        fh = open('testLabel.txt', 'r')
        self.testDocLabels = fh.readlines()
        fh.close()
        fh = open('testData.txt', 'r')
        testDataPoints = []
        testDataPoints = fh.readlines()
        fh.close()
        self.leaves = {}

        self.TrainMatrix = [[0] * (len(self.trainDocLabels)+1) for i in range(len(self.words)+1)]
        self.TestMatrix = [[0] * (len(self.testDocLabels)+1) for i in range(len(self.words)+1)]

        for line in trainDataPoints:
            docId = int(line.split(' ')[0])
            wordId = int(line.split(' ')[1])

            self.TrainMatrix[wordId][docId] = 1

        for line in testDataPoints:
            docId = int(line.split(' ')[0])
            wordId = int(line.split(' ')[1])
            self.TestMatrix[wordId][docId] = 1

    def learn(self):
        wordIds = len(self.words)
        docIds = len(self.trainDocLabels)
        minIEsplit = 1
        bestFeature = 1
        IE_pair = [0, 0]
        for i in range(1, wordIds+1):
            contains = n1 = n2 = n_1 = n_2 = 0
            for j in range(1, docIds+1):

                if self.TrainMatrix[i][j] == 1:
                    contains += self.TrainMatrix[i][j]
                    if int(self.trainDocLabels[j-1]) == 1:
                        n1 += 1
                    elif int(self.trainDocLabels[j-1]) == 2:
                        n2 += 1
                elif self.TrainMatrix[i][j] == 0:
                    if int(self.trainDocLabels[j-1]) == 1:
                        n_1 += 1
                    elif int(self.trainDocLabels[j-1]) == 2:
                        n_2 += 1
            E1 = (float(n1) / contains) if contains != 0 else 0
            E2 = (float(n2) / contains) if contains != 0 else 0

            IE1 = -1 * (E1 * math.log(E1, 2.0) if E1 != 0 else 0 +
                        E2 * math.log(E2, 2.0) if E2 != 0 else 0)
            E1 = (float(n_1) / (docIds - contains)) if (docIds - contains) != 0 else 0
            E2 = (float(n_2) / (docIds - contains)) if (docIds - contains) != 0 else 0
            IE2 = -1 * (E1 * math.log(E1, 2.0) if E1 != 0 else 0 +
                        E2 * math.log(E2, 2.0) if E2 != 0 else 0)

            fraction1 = float(contains)/docIds if self.featureSelection == 2 else 0.5
            fraction2 = float(docIds - contains) / docIds if self.featureSelection == 2 else 0.5
            IEsplit = fraction1 * IE1 + fraction2 * IE2
            if IEsplit < minIEsplit:
                minIEsplit = IEsplit
                bestFeature = i
                IE_pair[0] = IE1
                IE_pair[1] = IE2
        global root
        root = Node(minIEsplit, bestFeature, {bestFeature: True},
                    [x for x in range(1, 1501)], None, IE_pair[0], IE_pair[1], None, None)
        pqueue.put(root)

        # Number of nodes that the decision tree was tested
        testNodes = [25, 40, 50, 75, 100, 125]
        for inode in range(0, 10):
            split = pqueue.get()
            #print(split.bestFeature)
            #print('Selected')
            dataset = split.dataset

            dataset1 = []
            dataset2 = []
            wordID = split.bestFeature
            label1 = label2 = label_1 = label_2 = 0
            for i in range(0, len(dataset)):
                if self.TrainMatrix[wordID][dataset[i]] == 1:
                    dataset1.append(dataset[i])
                    if int(self.trainDocLabels[dataset[i]-1]) == 1:
                        label1 += 1
                    else:
                        label2 += 1
                else:
                    dataset2.append(dataset[i])
                    if int(self.trainDocLabels[dataset[i]-1]) == 1:
                        label_1 += 1
                    else:
                        label_2 += 1

            if len(dataset1) != 0:
                maxIG = 0
                maxFeature = 1
                entropy = [0, 0]
                wlist = copy.deepcopy(split.wlist)

                for i in range(1, wordIds + 1):
                    contains = n1 = n2 = n_1 = n_2 = 0

                    if wlist.get(i):
                        continue

                    for j in range(0, len(dataset1)):

                        if self.TrainMatrix[i][dataset1[j]] == 1:
                            contains += 1
                            if int(self.trainDocLabels[dataset1[j] - 1]) == 1:
                                n1 += 1
                            elif int(self.trainDocLabels[dataset1[j] - 1]) == 2:
                                n2 += 1
                        elif self.TrainMatrix[i][dataset1[j]] == 0:
                            if int(self.trainDocLabels[dataset1[j] - 1]) == 1:
                                n_1 += 1
                            elif int(self.trainDocLabels[dataset1[j] - 1]) == 2:
                                n_2 += 1
                    IE = split.IE1

                    E1 = (float(n1) / contains) if contains != 0 else 0
                    E2 = (float(n2) / contains) if contains != 0 else 0
                    IE1 = -1 * (E1 * math.log(E1, 2.0) if E1 != 0 else 0 +
                            E2 * math.log(E2, 2.0) if E2 != 0 else 0)

                    E1 = (float(n_1) / (len(dataset1) - contains)) if (len(dataset1) - contains) != 0 else 0
                    E2 = (float(n_2) / (len(dataset1) - contains)) if (len(dataset1) - contains) != 0 else 0
                    IE2 = -1 * (E1 * math.log(E1, 2.0) if E1 != 0 else 0 +
                        E2 * math.log(E2, 2.0) if E2 != 0 else 0)

                    fraction1 = float(contains) / len(dataset1) if self.featureSelection == 2 else 0.5
                    fraction2 = float(len(dataset1) - contains) / len(dataset1) if self.featureSelection == 2 else 0.5
                    IEsplit = fraction1 * IE1 + fraction2 * IE2
                    IG = IE - IEsplit
                    if IG > maxIG:
                        maxIG = IG
                        maxFeature = i
                        entropy[0] = IE1
                        entropy[1] = IE2
                wlist[maxFeature] = True
                node = Node(maxIG, maxFeature, wlist, dataset1, 1 if label1 > label2 else 2, entropy[0], entropy[1], None, None)
                split.left = node
                #print('Left is '+str(maxFeature)+'with IG '+str(maxIG))
                pqueue.put(node)

            if dataset2 != 0:
                maxIG = 0
                maxFeature = 1
                wlist = copy.deepcopy(split.wlist)
                for i in range(1, wordIds + 1):
                    contains = n1 = n2 = n_1 = n_2 = 0

                    if wlist.get(i):
                        continue

                    for j in range(0, len(dataset2)):

                        if self.TrainMatrix[i][dataset2[j]] == 1:
                            contains += 1
                            if int(self.trainDocLabels[dataset2[j] - 1]) == 1:
                                n1 += 1
                            elif int(self.trainDocLabels[dataset2[j] - 1]) == 2:
                                n2 += 1
                        elif self.TrainMatrix[i][dataset2[j]] == 0:
                            if int(self.trainDocLabels[dataset2[j] - 1]) == 1:
                                n_1 += 1
                            elif int(self.trainDocLabels[dataset2[j] - 1]) == 2:
                                n_2 += 1
                    IE = split.IE2

                    E1 = (float(n1) / contains) if contains != 0 else 0
                    E2 = (float(n2) / contains) if contains != 0 else 0
                    IE1 = -1 * (E1 * math.log(E1, 2.0) if E1 != 0 else 0 +
                                                                   E2 * math.log(E2, 2.0) if E2 != 0 else 0)
                    E1 = (float(n_1) / (len(dataset2) - contains)) if (len(dataset2) - contains) != 0 else 0
                    E2 = (float(n_2) / (len(dataset2) - contains)) if (len(dataset2) - contains) != 0 else 0
                    IE2 = -1 * (E1 * math.log(E1, 2.0) if E1 != 0 else 0 +
                                                                   E2 * math.log(E2, 2.0) if E2 != 0 else 0)

                    fraction1 = float(contains) / len(dataset2) if self.featureSelection == 2 else 0.5
                    fraction2 = float(len(dataset2) - contains) / len(dataset2) if self.featureSelection == 2 else 0.5
                    IEsplit = fraction1 * IE1 + fraction2 * IE2
                    IG = IE - IEsplit
                    if IG > maxIG:
                        maxIG = IG
                        maxFeature = i
                        entropy[0] = IE1
                        entropy[1] = IE2
                wlist[maxFeature] = True
                node = Node(maxIG, maxFeature, wlist, dataset2,
                        1 if label_1 > label_2 else 2, entropy[0], entropy[1], None, None)
                split.right = node
                #print('Right is ' + str(maxFeature) + 'with IG ' + str(maxIG))
                pqueue.put(node)

    def train_accuracy(self):
        docs = len(self.trainDocLabels)
        match = 0
        for doc in range(1, docs+1):
            rootNode = root
            while (rootNode is not None) and (rootNode.left is not None or rootNode.right is not None):
                if self.TrainMatrix[rootNode.bestFeature][doc] == 1 and rootNode.left is not None:
                    rootNode = rootNode.left
                elif self.TrainMatrix[rootNode.bestFeature][doc] == 0 and rootNode.right is not None:
                    rootNode = rootNode.right

            if self.leaves.get(rootNode.bestFeature) == int(self.trainDocLabels[doc-1]):
                match += 1
        print('Train Accuracy:'+str(float(match)/docs))

    def test_accuracy(self):
        docs = len(self.testDocLabels)
        match = 0
        for doc in range(1, docs + 1):
            rootNode = root
            while (rootNode is not None) and (rootNode.left is not None or rootNode.right is not None):
                if self.TestMatrix[rootNode.bestFeature][doc] == 1 and rootNode.left is not None:
                    rootNode = rootNode.left
                elif self.TestMatrix[rootNode.bestFeature][doc] == 0 and rootNode.right is not None:
                    rootNode = rootNode.right

            if self.leaves.get(rootNode.bestFeature) == int(self.testDocLabels[doc - 1]):
                match += 1
        print('Test Accuracy:' + str(float(match) / docs))

    def leaves_to_dictionary(self):
        while pqueue.qsize() > 0:
            node = pqueue.get()
            self.leaves[node.bestFeature] = node.PE

class Node:
    def __init__(self, maxIG, bestFeature, wlist, dataset, PE, IE1, IE2, left, right):
        self.maxIG = maxIG
        self.bestFeature = bestFeature
        self.wlist = wlist
        self.dataset = dataset
        self.PE = PE
        self.IE1 = IE1
        self.IE2 = IE2
        self.left = left
        self.right = right

    def __cmp__(self, other):
        return self.maxIG < other.maxIG


class DocLabelPair:
    def __init__(self, docId, labelId):
        self.docId = docId
        self.labelId = labelId


DT1 = DecisionTree(1)
DT2 = DecisionTree(2)
DT1.learn()
DT2.learn()

DT1.leaves_to_dictionary()
DT2.leaves_to_dictionary()
DT1.train_accuracy()
DT1.test_accuracy()
DT2.train_accuracy()
DT2.test_accuracy()



