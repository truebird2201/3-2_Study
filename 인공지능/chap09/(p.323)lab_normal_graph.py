import numpy as np
import matplotlib.pyplot as plt

m = 10; sigma = 2
x1 = np.random.randn(10000)
x2 = m+sigma*np.random.randn(10000)

plt.figure(figsize=(10,6))
plt.hist(x1, bins=20, alpha=0.4)		# 20개의 상자를 이용하여 히스토그램 계산
plt.hist(x2, bins=20, alpha=0.4)
plt.show()