import numpy as np

# 시그모이드 함수
def actf(x):
	return 1/(1+np.exp(-x))

# 시그모이드 함수의 미분값
def actf_deriv(x):
	    return x*(1-x)
    
# XOR 연산을 위한 4행*2열의 입력 행렬
# 마지막 열은 바이어스를 나타낸다. 
X = np.array([[0,0,1], [0,1,1], [1,0,1], [1,1,1]])
                
# XOR 연산을 위한 4행*1열의 목표 행렬
y = np.array([[0], [1], [1], [0]])

np.random.seed(5)

inputs = 3	# 입력층의 노드 개수
hiddens = 6	# 은닉층의 노드 개수
outputs = 1	# 출력층의 노드 개수

# 가중치를 –1.0에서 1.0 사이의 난수로 초기화한다.
weight0 = 2*np.random.random((inputs, hiddens))-1   
weight1 = 2*np.random.random((hiddens, outputs))-1  

# 반복한다. 
for i in range(10000):

    # 순방향 계산
    layer0 = X				# 입력을 layer0에 대입한다. 
    net1 = np.dot(layer0, weight0)	# 행렬의 곱을 계산한다. 
    layer1 = actf(net1)		# 활성화 함수를 적용한다. 
    layer1[:,-1] = 1.0      	#  마지막 열은 바이어스를 나타낸다. 1.0으로 만든다.
    net2 = np.dot(layer1, weight1)	# 행렬의 곱을 계산한다. 
    layer2 = actf(net2)		# 활성화 함수를 적용한다. 

    # 출력층에서의 오차를 계산한다. 
    layer2_error = layer2-y
        
    # 출력층에서의 델타값을 계산한다. 
    layer2_delta = layer2_error*actf_deriv(layer2)

    # 은닉층에서의 오차를 계산한다.
    # 여기서 T는 행렬의 전치를 의미한다. 
    # 역방향으로 오차를 전파할 때는 반대방향이므로 행렬이 전치되어야 한다. 
    layer1_error = np.dot(layer2_delta, weight1.T)
    
    # 은닉층에서의 델타를 계산한다.
    layer1_delta = layer1_error*actf_deriv(layer1)

    # 은닉층->출력층을 연결하는 가중치를 수정한다. 
    weight1 += -0.2*np.dot(layer1.T, layer2_delta)

    # 입력층->은닉층을 연결하는 가중치를 수정한다. 
    weight0 += -0.2*np.dot(layer0.T, layer1_delta)
print(layer2)			# 현재 출력층의 값을 출력한다. 