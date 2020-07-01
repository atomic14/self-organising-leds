frontend_requirements:
	cd frontend && yarn

build_frontend: frontend_requirements
	cd frontend && yarn build

compress_frontend:
	cd frontend && yarn export

build_firmware:
	source ./venv/bin/activate && cd firmware && platformio run

build: build_frontend build_firmware

upload_firmware:
	source ./venv/bin/activate && cd firmware && platformio run --target upload

upload_fs: build_frontend compress_frontend
	source ./venv/bin/activate && cd firmware && platformio run --target uploadfs


upload: upload_fs upload_firmware