import matplotlib.pylab as plt
import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn import datasets

# 당뇨병 데이터 세트를 적재한다. 
diabetes = datasets.load_diabetes()

# 학습 데이터와 테스트 데이터를 분리한다. 
from sklearn.model_selection import train_test_split
X_train, X_test, y_train, y_test = train_test_split(diabetes.data, diabetes.target, test_size=0.2, random_state=0)

model = LinearRegression()
model.fit(X_train, y_train)

# 테스트 데이터로 예측해보자. 
y_pred = model.predict(X_test) 

# 실제 데이터와 예측 데이터를 비교해보자. 
plt.plot(y_test, y_pred, '.')

# 직선을 그리기 위하여 완벽한 선형 데이터를 생성한다. 
x = np.linspace(0, 330, 100)
y = x
plt.plot(x, y)
plt.show()