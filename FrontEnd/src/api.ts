import axios from "axios";

const BASE_URL = "http://localhost:3000";

const apiClient = axios.create({
  baseURL: BASE_URL,
  headers: {
    "Content-Type": "application/json",
  },
});

export const getAllVehicles = async () => {
  try {
    const response = await apiClient.get("/vehicles");
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در دریافت لیست وسایل نقلیه!";
  }
};


export const getVehicle = async (id: string | number) => {
  try {
    const response = await apiClient.get(`/vehicles/${id}`);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در دریافت اطلاعات وسیله نقلیه!";
  }
};

export const createVehicle = async (vehicleData: any) => {
  try {
    const response = await apiClient.post("/vehicles", vehicleData);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در ثبت وسیله نقلیه جدید!";
  }
};

export const updateVehicle = async (id: string | number, vehicleData: any) => {
  try {
    const response = await apiClient.put(`/vehicles/${id}`, vehicleData);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در به‌روزرسانی اطلاعات وسیله نقلیه!";
  }
};


export const deleteVehicle = async (id: string | number) => {
  try {
    const response = await apiClient.delete(`/vehicles/${id}`);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در حذف وسیله نقلیه!";
  }
};

export default apiClient;
