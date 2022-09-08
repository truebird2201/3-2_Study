from sklearn.datasets import load_iris
iris = load_iris()

from sklearn.model_selection import train_test_split

# 입력과 출력을 설정한다. 
X = iris.data
y = iris.target

# 전체 데이터를 학습 데이터와 테스트 데이터 비율 (80:20)으로 분할한다. 
X_train,X_test,y_train,y_test = train_test_split(X,y,test_size=0.2,random_state=4)

from sklearn.neighbors import KNeighborsClassifier
from sklearn import metrics

# 학습 단계
knn = KNeighborsClassifier(n_neighbors=6)
knn.fit(X_train, y_train)

# 테스트 단계
y_pred = knn.predict(X_test)

# 정확도 점수 출력
scores = metrics.accuracy_score(y_test, y_pred)
print(scores)