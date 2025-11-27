import axios from "axios";

export const apiClient = axios.create({
  baseURL: "http://api.smartparking.niflheimdevs.ir:8080/v1",
  headers: {
    "Content-Type": "application/json",
  },
});

export interface VehiclePayload {
  owner_name: string;
  owner_contact: string;
  plate: string;
  rfid_id: string;
  vehicle_type: string;
}

// =====================================================
// GET ALL VEHICLES
// =====================================================
export const getAllVehicles = async () => {
  try {
    const response = await apiClient.get("/vehicles");
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در دریافت لیست وسایل نقلیه!";
  }
};

// =====================================================
// GET SPECIFIC VEHICLE
// =====================================================
export const getVehicle = async (id: string | number) => {
  try {
    const response = await apiClient.get(`/vehicles/${id}`);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در دریافت اطلاعات وسیله نقلیه!";
  }
};

// =====================================================
// CREATE VEHICLE
// =====================================================
export const createVehicle = async (vehicleData: VehiclePayload) => {
  try {
    const response = await apiClient.post("/vehicles", vehicleData);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در ثبت وسیله نقلیه جدید!";
  }
};

// =====================================================
// UPDATE VEHICLE
// =====================================================
export const updateVehicle = async (id: string | number, vehicleData: VehiclePayload) => {
  try {
    const response = await apiClient.put(`/vehicles/${id}`, vehicleData);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در به‌روزرسانی اطلاعات وسیله نقلیه!";
  }
};

// =====================================================
// DELETE VEHICLE
// =====================================================
export const deleteVehicle = async (id: string | number) => {
  try {
    const response = await apiClient.delete(`/vehicles/${id}`);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در حذف وسیله نقلیه!";
  }
};

export default apiClient;
