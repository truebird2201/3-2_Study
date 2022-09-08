import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt 

# 가상적인 데이터 생성 
X = data = np.linspace(1,2,200)	# 시작값=1, 종료값=2, 개수=200
y = X*4 + np.random.randn(200) * 0.3	# x를 4배로 하고 편차 0.3정도의 가우시안 잡음추가

model = tf.keras.models.Sequential()
model.add(tf.keras.layers.Dense(1, input_dim=1, activation='linear'))
model.compile(optimizer='sgd', loss='mse', metrics=['mse'])
model.fit(X, y, batch_size=1, epochs=30)

predict = model.predict(data)

plt.plot(data, predict, 'b', data, y, 'k.') # 첫 번째 그래프는 파란색 마커로
plt.show()			      	      # 두 번째 그래프는 검정색 .으로 그린다.