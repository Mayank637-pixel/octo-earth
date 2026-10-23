FROM gcc:14 AS core-build
RUN apt-get update && apt-get install -y --no-install-recommends cmake && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY core-cpp core-cpp
COPY contracts contracts
RUN cmake -S core-cpp -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --parallel

FROM python:3.12-slim
WORKDIR /app
COPY backend-py/pyproject.toml .
COPY backend-py/app app
RUN pip install --no-cache-dir .
COPY --from=core-build /src/build/liboctocore.so /app/liboctocore.so
ENV OCTOCORE_LIB_PATH=/app/liboctocore.so
EXPOSE 8000
CMD ["uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "8000"]
